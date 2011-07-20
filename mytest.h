#ifndef _MYTEST_H
#define _MYTEST_H

#define SOME_CONSTANT 1

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

struct Point {
    float x;
    float y;
};

struct Rect {
    struct Point a;
    struct Point b;
};

union Something {
    int i;
    float f;
};

float area(const struct Rect *r);

_returns_(42) int rect_from_center(
        _out_ struct Rect *r,
        const struct Point *c,
        _default_(h) const float w,
        _default_(w) const float h
        );

_returns_(0) int multiply(
        _value_(a.shape[0]) const int w,
        _value_(a.shape[1]) const int h,
        _out_ _size_(w,h) float *result,
        _size_(w,h) const float *a,
        _size_(w,h) const float *b
        );

#endif

