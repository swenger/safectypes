import ctypes
import os
import re

import numpy
import pygccxml

# see also http://starship.python.net/crew/theller/ctypes/old/codegen.html

# TODO handle #defined constants
# TODO suppress pygccxml output and popen deprecation warning

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
        return ctypes_from_gccxml(lib, getattr_rec(lib, [x for x in t.decl_string.split("::") if x])) # resolve namespace into nested modules
    if type(t) == type(ctypes.Structure) and issubclass(t, ctypes.Structure):
        return t
    # TODO if necessary, add: compound, class, enumeration, reference, ellipsis, free_function_type, ctypes.*
    raise NotImplementedError("no ctypes equivalent for %s %s" % (t.__class__.__name__, t.decl_string))

def numpy_from_ctypes(t):
    d = {
            #ctypes.c_wchar: numpy.wchar,
            ctypes.c_bool: numpy.bool,
            ctypes.c_double: numpy.double,
            ctypes.c_longlong: numpy.longlong,
            ctypes.c_short: numpy.short,
            #ctypes.c_ulong: numpy.ulong,
            ctypes.c_int: numpy.int,
            ctypes.c_int32: numpy.int32,
            #ctypes.c_buffer: numpy.buffer,
            ctypes.c_longdouble: numpy.longdouble,
            ctypes.c_long: numpy.long,
            #ctypes.c_char_p: numpy.char_p,
            ctypes.c_ushort: numpy.ushort,
            #ctypes.c_wchar_p: numpy.wchar_p,
            ctypes.c_int64: numpy.int64,
            ctypes.c_int16: numpy.int16,
            ctypes.c_uint64: numpy.uint64,
            ctypes.c_ubyte: numpy.ubyte,
            ctypes.c_char: numpy.char,
            ctypes.c_uint32: numpy.uint32,
            ctypes.c_float: numpy.float32,
            ctypes.c_int8: numpy.int8,
            ctypes.c_byte: numpy.byte,
            ctypes.c_ulonglong: numpy.ulonglong,
            #ctypes.c_voidp: numpy.voidp,
            ctypes.c_uint16: numpy.uint16,
            ctypes.c_uint: numpy.uint,
            #ctypes.c_void_p: numpy.void_p,
            #ctypes.c_size_t: numpy.size_t,
            ctypes.c_uint8: numpy.uint8,
            }
    return d[t]

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
    def parse_parentheses(s, separator=","):
        objs = [""]
        level = 0
        for c in s:
            if c == "(":
                level += 1
                objs[-1] = objs[-1] + c
            elif c == ")":
                level -= 1
                objs[-1] = objs[-1] + c
            elif c == separator and level == 0:
                objs.append("")
            else:
                objs[-1] = objs[-1] + c
        objs = [x.strip() for x in objs]
        if len(objs) == 1 and len(objs[0]) == 0:
            return []
        else:
            return objs

    def __init__(self, lib, func, declaration):
        self.func = func
        self.name = declaration.name
        self.returns = None
        self.arguments = [Argument(a.name) for a in declaration.arguments]

        # parse gccxml attributes for function from header file
        attributes = CallHandler.parse_parentheses(declaration.attributes or "")
        for attribute in attributes:
            attribute_type, attribute_params = CallHandler.attribute_re.match(attribute).groups()
            if attribute_type != "gccxml":
                continue
            attribute_params = CallHandler.parse_parentheses(attribute_params)
            attribute_name, attribute_params = attribute_params[0], attribute_params[1:]

            if attribute_name == "returns":
                assert len(attribute_params) == 1
                self.returns = attribute_params[0]

        # parse gccxml attributes for arguments from header file
        for pos, argument in enumerate(declaration.arguments or []):
            arg_attributes = CallHandler.parse_parentheses(argument.attributes or "", " ")
            for attribute in arg_attributes:
                attribute_type, attribute_params = CallHandler.attribute_re.match(attribute).groups()
                if attribute_type != "gccxml":
                    continue
                attribute_params = CallHandler.parse_parentheses(attribute_params)
                attribute_name, attribute_params = attribute_params[0], attribute_params[1:]

                if attribute_name == "default":
                    assert len(attribute_params) == 1
                    self.arguments[pos].default_value = attribute_params[0]
                elif attribute_name == "size":
                    assert len(attribute_params) > 0
                    self.arguments[pos].size = attribute_params
                    try:
                        self.arguments[pos].contained_type = ctypes_from_gccxml(lib, argument.type.base)
                    except AttributeError:
                        raise TypeError("argument %s has a size attribute but is not of pointer type" % argument.name)
                elif attribute_name == "out":
                    assert len(attribute_params) == 0
                    self.arguments[pos].out = True
                    try:
                        self.arguments[pos].contained_type = ctypes_from_gccxml(lib, argument.type.base)
                    except AttributeError:
                        raise TypeError("argument %s has an out attribute but is not of pointer type" % argument.name)
                elif attribute_name == "value":
                    assert len(attribute_params) == 1
                    self.arguments[pos].value = attribute_params[0]

    def __call__(self, *args, **kwargs):
        n_args_max = sum("value" not in a.__dict__ for a in self.arguments)
        n_args_min = sum("value" not in a.__dict__ and "default_value" not in a.__dict__ and "out" not in a.__dict__ for a in self.arguments)
        n_args_given = len(args)

        args = list(args)
        arguments = []
        args_to_eval = []
        args_to_create = []
        for arg in self.arguments:
            # try to set predefined value
            try:
                args_to_eval.append((len(arguments), arg.name, arg.value))
                arguments.append(None)
                continue
            except AttributeError:
                pass

            # try to read positional argument
            try:
                if "out" not in arg.__dict__: # out arguments should be optional and are therefore never positional
                    arguments.append(args.pop(0))
                    if arg.name in kwargs:
                        raise TypeError("%s() got multiple values for keyword argument '%s'" % (self.name, arg.name))
                    kwargs[arg.name] = arguments[-1]
                    continue
            except IndexError:
                pass

            # try to read keyword argument
            try:
                arguments.append(kwargs[arg.name])
                continue
            except KeyError:
                pass

            # try to read default argument
            try:
                args_to_eval.append((len(arguments), arg.name, arg.default_value))
                arguments.append(None)
                continue
            except AttributeError:
                pass

            # handle output arguments
            if "out" in arg.__dict__:
                args_to_create.append((len(arguments), arg))
                arguments.append(None)
                continue

            # argument is missing
            raise TypeError("%s() takes at least %d non-keyword arguments (%d given)" % (self.name, n_args_min, n_args_given))

        # make sure no positional arguments remain
        if args:
            raise TypeError("%s() takes at most %d non-keyword arguments (%d given)" % (self.name, n_args_max, n_args_given))

        # evaluate arguments which may depend on each other TODO evaluate size, too?
        no_change = 0
        while args_to_eval:
            if no_change >= len(args_to_eval):
                raise RuntimeError("circular dependency in arguments")
            pos, name, default = args_to_eval.pop()
            try:
                kwargs[name] = arguments[pos] = eval(default, kwargs)
                no_change = 0
            except NameError:
                args_to_eval.append((pos, name, default))
                no_change += 1

        # create out parameters
        for pos, arg in args_to_create:
            try:
                shape = tuple(eval(x, kwargs) for x in arg.size)
                arguments[pos] = numpy.empty(shape, dtype=numpy_from_ctypes(arg.contained_type))
            except AttributeError:
                arguments[pos] = arg.contained_type()

        # handle arrays
        for pos, arg in enumerate(self.arguments):
            if "size" in arg.__dict__:
                # tell ctypes to check array properties
                l = self.func.argtypes
                l[pos] = numpy.ctypeslib.ndpointer(
                        ndim=len(self.arguments[pos].size),
                        dtype=numpy_from_ctypes(self.arguments[pos].contained_type),
                        flags="CONTIGUOUS",
                        shape=tuple(eval(x, kwargs) for x in arg.size),
                        )
                self.func.argtypes = l

        # call function
        retval = self.func(*arguments)

        # build output list
        outputs = []
        for pos, arg in enumerate(self.arguments):
            if "out" in arg.__dict__:
                outputs.append(arguments[pos])
    
        # check return value
        if self.returns is not None:
            returns = eval(self.returns, kwargs)
            if retval != returns:
                raise RuntimeError("%s returned %s instead of %s (%s)" % (self.name, repr(retval), self.returns, returns))
            if len(outputs) == 1:
                return outputs[0]
            else:
                return tuple(outputs)
        else:
            return (retval,) + tuple(outputs)

def load_dll(lib_name, header_name):
    lib = ctypes.CDLL(lib_name)
    declarations = pygccxml.parser.parse([header_name])[0].declarations

    # TODO handle C++ name mangling and function overloading (possibly ambiguous!)
    for declaration in sorted(declarations, key=lambda x: x.location.line if x.location else 0):
        if not declaration.location or os.path.basename(declaration.location.file_name) == "gccxml_builtins.h": # skip default includes
            continue
        if isinstance(declaration, pygccxml.declarations.enumeration_t):
            setattr(lib, declaration.name, type(declaration.name, (object,), dict(declaration.values)))
        elif isinstance(declaration, pygccxml.declarations.typedef_t):
            setattr(lib, declaration.name, declaration.type)
        elif isinstance(declaration, pygccxml.declarations.class_t): # TODO resolve circular dependencies and forward declarations
            # TODO currently, this only handles structs (no member functions); add creating of member functions
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
        elif isinstance(declaration, pygccxml.declarations.free_function_t):
            try:
                func = getattr(lib, declaration.name)
                func.restype = ctypes_from_gccxml(lib, declaration.return_type)
                func.argtypes = [ctypes_from_gccxml(lib, a.type) for a in declaration.arguments]
                setattr(lib, declaration.name, CallHandler(lib, func, declaration))
            except (AttributeError, NotImplementedError):
                pass
        # TODO handle namespaces (create nested modules)

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

    """
    dll = load_dll("libc.so.6", "/usr/include/stdio.h")
    i = ctypes.c_int()
    f = ctypes.c_float()
    s = ctypes.create_string_buffer('\000' * 32)
    dll.sscanf("1 3.14 Hello", "%d %f %s", ctypes.byref(i), ctypes.byref(f), s)
    print i.value, f.value, s.value
    """

