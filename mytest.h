#ifndef _MYTEST_H
#define _MYTEST_H

#define SOME_CONSTANT 1

#ifdef __GCCXML__
#define _out_ __attribute((gccxml("out")))
#define _size_(...) __attribute((gccxml("size", #__VA_ARGS__)))
#define _returns_(X) __attribute((gccxml("returns", #X)))
#else
#define _out_
#define _returns_(X)
#define _size_(...)
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

_returns_(42) int rect_from_center(_out_ struct Rect *r, const struct Point *c, const float w=1, const float h=1);

_returns_(0) int multiply(const int w, const int h, _out_ _size_(w,h) float *result, _size_(w,h) const float *a, _size_(w,h) const float *b);

#endif

