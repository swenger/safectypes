#ifndef _SAFECTYPES_H
#define _SAFECTYPES_H

#ifdef __GCCXML__
#define _out_ __attribute((gccxml("out")))
#define _size_(...) __attribute((gccxml("size", #__VA_ARGS__)))
#define _returns_(X) __attribute((gccxml("returns", #X)))
#define _default_(X) __attribute((gccxml("default", #X)))
#define _value_(X) __attribute((gccxml("value", #X)))
#else
#define _out_
#define _size_(...)
#define _returns_(X)
#define _default_(X)
#define _value_(X)
#endif

#endif

