#ifndef _MYTEST_H
#define _MYTEST_H

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

void rect_from_center(struct Rect *r, const struct Point *c, const float w, const float h);

#endif

