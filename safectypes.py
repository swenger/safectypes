import ctypes
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
        return getattr_rec(lib, [x for x in t.decl_string.split("::") if x]) # resolve namespace
    # TODO compound, class, enumeration, reference, ellipsis
    raise NotImplementedError("no ctypes equivalent for %s %s" % (t.__class__.__name__, t.decl_string))

class ValidateEqual(object):
    def __init__(self, value, exc=Exception):
        self.value, self.exc = value, exc
    def __call__(self, result, func, arguments):
        if result != self.value:
            raise self.exc("%s returned %s" % (func.__name__, result))

def load_dll(lib_name, header_name):
    lib = ctypes.CDLL(lib_name)
    header = pygccxml.parser.parse([header_name])[0]

    # classes
    try:
        for c in sorted(header.classes()):
            fields = []
            for v in c.variables():
                fields.append((v.name, ctypes_from_gccxml(lib, v.type)))
            setattr(lib, c.name, type(c.name, (ctypes.Structure,), {"_fields_": fields}))
            for alias in c.aliases:
                setattr(lib, alias.name, getattr(lib, c.name))
    except RuntimeError:
        pass # no classes

    # enums
    try:
        for e in sorted(header.enums()):
            setattr(lib, e.name, type(e.name, (object,), dict(e.values)))
    except RuntimeError:
        pass # no enums

    # functions
    try:
        for f in sorted(header.free_functions()):
            try:
                func = getattr(lib, f.name)
                func.restype = ctypes_from_gccxml(lib, f.return_type)
                func.argtypes = [ctypes_from_gccxml(lib, a.type) for a in f.arguments]
            except (AttributeError, NotImplementedError):
                pass
    except RuntimeError:
        pass # no functions

    return lib

if __name__ == "__main__":
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

