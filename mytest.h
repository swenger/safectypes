#ifndef _MYTEST_H
#define _MYTEST_H

#include <math.h>

#include "safectypes.h"

struct Point {
    float x;
    float y;
};

struct Rect {
    struct Point a;
    struct Point b;
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

_returns_(0) int mysincos(double value, _out_ double *s, _out_ double *c) {
	*s= sin(value);
	*c= cos(value);
	return 0;
}

#endif

