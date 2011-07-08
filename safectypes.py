import ctypes
import pygccxml

def ctypes_from_gccxml(t):
    """Convert a pygccxml type to a ctypes type."""
    if isinstance(t, pygccxml.declarations.const_t):
        return ctypes_from_gccxml(t.base)
    if isinstance(t, pygccxml.declarations.pointer_t):
        if isinstance(t.base, pygccxml.declarations.void_t):
            return ctypes.c_void_p
        if isinstance(t.base, pygccxml.declarations.char_t):
            return ctypes.c_char_p
        if isinstance(t.base, pygccxml.declarations.wchar_t):
            return ctypes.c_wchar_p
        return ctypes.POINTER(ctypes_from_gccxml(t.base))
    if isinstance(t, pygccxml.declarations.array_t):
        return ctypes.ARRAY(ctypes_from_gccxml(t.base), t.size)
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
    # TODO compound, class, declarated, enumeration, reference
    raise NotImplementedError("no ctypes equivalent for %s %s" % (t.__class__.__name__, t.decl_string))

class ValidateEqual(object):
    def __init__(self, value, exc=Exception):
        self.value, self.exc = value, exc
    def __call__(self, result, func, arguments):
        if result != self.value:
            raise self.exc("%s returned %s" % (func.__name__, result))

def load_dll(lib_name, header_name):
    lib = ctypes.CDLL(lib_name)
    header = pygccxml.parser.parse([header_name])

    for declaration in sorted(header[0].declarations):
        if isinstance(declaration, pygccxml.declarations.calldef.free_function_t):
            try:
                f = getattr(lib, declaration.name) # TODO C++ name mangling?
            except AttributeError:
                continue

            try:
                f.argtypes = [ctypes_from_gccxml(a.type) for a in declaration.arguments]
                f.restype = ctypes_from_gccxml(declaration.return_type)
                #f.errcheck = ValidateEqual(20002) # DEBUG
            except NotImplementedError, e:
                print "ignoring %s %s: %s" % (declaration.decl_string, declaration.name, e)
        else:
            print "ignoring %s %s" % (declaration.__class__.__name__, declaration.name) # TODO handle structs, enums, typedefs etc.

    return lib

if __name__ == "__main__":
    dll = load_dll("libm.so", "/usr/include/math.h")
    print dll.cos(3.14)

