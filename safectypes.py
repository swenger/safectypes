import ctypes
import os
import re
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

class Argument(object):
    def __init__(self, name):
        self.name = name

class CallHandler(object):
    attribute_re = re.compile(r'\A(\w+)\s*\(\s*(.*)\s*\)\s*')

    @staticmethod
    def parse_parentheses(s):
        objs = [""]
        level = 0
        for c in s:
            if c == "(":
                level += 1
                objs[-1] = objs[-1] + c
            elif c == ")":
                level -= 1
                objs[-1] = objs[-1] + c
            elif c == "," and level == 0:
                objs.append("")
            else:
                objs[-1] = objs[-1] + c
        objs = [x.strip() for x in objs]
        if len(objs) == 1 and len(objs[0]) == 0:
            return []
        else:
            return objs

    def __init__(self, func, declaration):
        self.func = func
        self.name = declaration.name
        self.returns = None
        self.arguments = [Argument(a.name) for a in declaration.arguments]

        attributes = CallHandler.parse_parentheses(declaration.attributes or "")
        for attribute in attributes:
            attribute_type, attribute_params = CallHandler.attribute_re.match(attribute).groups()
            if attribute_type != "gccxml":
                continue
            attribute_params = CallHandler.parse_parentheses(attribute_params)
            attribute_name, attribute_params = attribute_params[0], attribute_params[1:]

            if attribute_name == "returns":
                assert len(attribute_params) == 1
                self.returns = int(attribute_params[0]) # TODO cast to appropriate declaration.return_type

        for pos, argument in enumerate(declaration.arguments or []):
            arg_attributes = CallHandler.parse_parentheses(argument.attributes or "")
            for attribute in arg_attributes:
                attribute_type, attribute_params = CallHandler.attribute_re.match(attribute).groups()
                if attribute_type != "gccxml":
                    continue
                attribute_params = CallHandler.parse_parentheses(attribute_params)
                attribute_name, attribute_params = attribute_params[0], attribute_params[1:]

                if attribute_name == "default":
                    assert len(attribute_params) == 1
                    self.arguments[pos].default_value = int(attribute_params[0]) # TODO cast to appropriate argument.type
                elif attribute_name == "size":
                    assert len(attribute_params) > 0
                    self.arguments[pos].size = attribute_params[1:]
                elif attribute_name == "out":
                    assert len(attribute_params) == 0
                    self.arguments[pos].out = True

    def __call__(self, *args, **kwargs):
        if len(args) > len(self.arguments):
            raise RuntimeError("too many arguments")
        args = list(args)
        for arg in self.arguments[:len(args)]:
            if arg.name in kwargs:
                raise RuntimeError("argument '%s' specified twice" % arg.name)
        for arg in self.arguments[len(args):]:
            if arg.name in kwargs:
                args.append(kwargs[arg.name])
            else:
                try:
                    args.append(arg.default_value)
                except AttributeError:
                    raise RuntimeError("argument '%s' has to be specified" % arg.name)

        # TODO handle out parameters and sizes
        retval = self.func(*args)
        if self.returns is not None:
            if retval != self.returns:
                raise RuntimeError("%s returned %s instead of %s" % (self.name, repr(retval), repr(self.returns)))
        else:
            return retval

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
                func.argtypes = [ctypes_from_gccxml(lib, a.type) for a in declaration.arguments]
                setattr(lib, declaration.name, CallHandler(func, declaration))
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

