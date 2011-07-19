import ctypes
import os
import pygccxml

# see also http://starship.python.net/crew/theller/ctypes/old/codegen.html

def getattr_rec(obj, names):
    if len(names) == 0:
        return obj
    else:
        return getattr_rec(getattr(obj, names[0]), names[1:])

def ctypes_from_gccxml(lib, t):
    """Convert a pygccxml type to a ctypes type."""
    if isinstance(t, pygccxml.declarations.restrict_t):
        return ctypes_from_gccxml(lib, t.base)
    if isinstance(t, pygccxml.declarations.const_t):
        return ctypes_from_gccxml(lib, t.base)
    if isinstance(t, pygccxml.declarations.pointer_t):
        if isinstance(t.base, pygccxml.declarations.void_t):
            return ctypes.c_void_p
        if isinstance(t.base, pygccxml.declarations.char_t):
            return ctypes.c_char_p
        if isinstance(t.base, pygccxml.declarations.wchar_t):
            return ctypes.c_wchar_p
        return ctypes.POINTER(ctypes_from_gccxml(lib, t.base))
    if isinstance(t, pygccxml.declarations.array_t):
        return ctypes.ARRAY(ctypes_from_gccxml(lib, t.base), t.size)
    if isinstance(t, pygccxml.declarations.bool_t):
        return ctypes.c_bool
    if isinstance(t, pygccxml.declarations.char_t):
        return ctypes.c_char
    if isinstance(t, pygccxml.declarations.double_t):
        return ctypes.c_double
    if isinstance(t, pygccxml.declarations.float_t):
        return ctypes.c_float
    if isinstance(t, pygccxml.declarations.int_t):
        return ctypes.c_int
    if isinstance(t, pygccxml.declarations.long_double_t):
        return ctypes.c_longdouble
    if isinstance(t, pygccxml.declarations.long_int_t):
        return ctypes.c_long
    if isinstance(t, pygccxml.declarations.long_long_int_t):
        return ctypes.c_longlong
    if isinstance(t, pygccxml.declarations.long_long_unsigned_int_t):
        return ctypes.c_ulonglong
    if isinstance(t, pygccxml.declarations.long_unsigned_int_t):
        return ctypes.c_ulong
    if isinstance(t, pygccxml.declarations.short_int_t):
        return ctypes.c_short
    if isinstance(t, pygccxml.declarations.short_unsigned_int_t):
        return ctypes.c_ushort
    if isinstance(t, pygccxml.declarations.signed_char_t):
        return ctypes.c_char
    if isinstance(t, pygccxml.declarations.unsigned_char_t):
        return ctypes.c_ubyte
    if isinstance(t, pygccxml.declarations.unsigned_int_t):
        return ctypes.c_uint
    if isinstance(t, pygccxml.declarations.wchar_t):
        return ctypes.c_wchar
    if isinstance(t, pygccxml.declarations.void_t):
        return None
    if isinstance(t, pygccxml.declarations.cpptypes.declarated_t):
        return ctypes_from_gccxml(lib, getattr_rec(lib, [x for x in t.decl_string.split("::") if x])) # resolve namespace
    if type(t) == type(ctypes.Structure) and issubclass(t, ctypes.Structure): # TODO etc
        return t
    # TODO compound, class, enumeration, reference, ellipsis, free_function_type
    raise NotImplementedError("no ctypes equivalent for %s %s" % (t.__class__.__name__, t.decl_string))

class ValidateEqual(object):
    def __init__(self, value, exc=Exception):
        self.value, self.exc = value, exc
    def __call__(self, result, func, arguments):
        if result != self.value:
            raise self.exc("%s returned %s" % (func.__name__, result))

def load_dll(lib_name, header_name):
    lib = ctypes.CDLL(lib_name)
    declarations = pygccxml.parser.parse([header_name])[0].declarations

    for declaration in sorted(declarations, key=lambda x: x.location.line if x.location else 0):
        if not declaration.location or os.path.basename(declaration.location.file_name) == "gccxml_builtins.h": # skip default includes
            continue
        if isinstance(declaration, pygccxml.declarations.enumeration_t):
            setattr(lib, declaration.name, type(declaration.name, (object,), dict(declaration.values)))
        elif isinstance(declaration, pygccxml.declarations.typedef_t):
            setattr(lib, declaration.name, declaration.type)
        elif isinstance(declaration, pygccxml.declarations.class_t): # TODO resolve circular dependencies
            try:
                fields = []
                try:
                    for v in declaration.variables():
                        fields.append((v.name, ctypes_from_gccxml(lib, v.type)))
                except RuntimeError:
                    pass # no variables
                setattr(lib, declaration.name, type(declaration.name, (ctypes.Structure,), {"_fields_": fields}))
            except NotImplementedError:
                print "skipping class %s" % declaration.name
        if isinstance(declaration, pygccxml.declarations.free_function_t):
            try:
                func = getattr(lib, declaration.name)
                func.restype = ctypes_from_gccxml(lib, declaration.return_type)
                # TODO use a.name for named arguments, a.default_value for default values, a.attributes for gccxml attributes
                func.argtypes = [ctypes_from_gccxml(lib, a.type) for a in declaration.arguments]
                print declaration.name, declaration.attributes # DEBUG
                for a in declaration.arguments: print declaration.name, a.name, a.attributes # DEBUG
            except (AttributeError, NotImplementedError):
                pass

    return lib

if __name__ == "__main__":
    """
    dll = load_dll("libandor.so", "/usr/local/include/atmcdLXd.h")
    for key, value in sorted(dll.__dict__.items()):
        if not key.startswith("_"):
            try:
                print "%s %s(%s)" % (value.restype.__name__, key, ", ".join(a.__name__ for a in value.argtypes or []))
            except AttributeError:
                pass
    """

    dll = load_dll("mytest.so", "mytest.h")
    p1 = dll.Point(1.0, 2.0)
    p2 = dll.Point(4.0, 1.0)
    r1 = dll.Rect(p1, p2)
    f1 = dll.area(r1)
    print f1, (r1.a.x - r1.b.x) * (r1.a.y - r1.b.y);
    r2 = dll.Rect()
    p3 = dll.Point(5, 6)
    w = 3
    h = 1
    dll.rect_from_center(r2, p3, w, h)
    print (r2.a.x, r2.a.y, r2.b.x, r2.b.y), (p3.x - 0.5 * w, p3.y - 0.5 * h, p3.x + 0.5 * w, p3.y + 0.5 * h)

    """
    dll = load_dll("libc.so.6", "/usr/include/stdio.h")
    i = ctypes.c_int()
    f = ctypes.c_float()
    s = ctypes.create_string_buffer('\000' * 32)
    dll.sscanf("1 3.14 Hello", "%d %f %s", ctypes.byref(i), ctypes.byref(f), s)
    print i.value, f.value, s.value
    """

