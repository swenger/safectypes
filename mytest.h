#ifndef _MYTEST_H
#define _MYTEST_H

#define SOME_CONSTANT 1

#ifdef __GCCXML__
#define _out_ __attribute((gccxml("out")))
#else
#define _out_
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

void rect_from_center(_out_ struct Rect *r, const struct Point *c, const float w, const float h);

#endif

