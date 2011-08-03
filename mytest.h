#ifndef _MYTEST_H
#define _MYTEST_H

#include <math.h>

#include "safectypes.h"

#define OK 42
const int okay = 0;

struct Point {
    float x;
    float y;
};

struct Rect {
    struct Point a;
    struct Point b;
};

int print_string(_string_ const char *s);

void print_string_fix(_string_ _size_(l) const char *s, _value_(len(s)) int l);

void get_name(_out_ _string_ _size_(l) char *s, int l);

float area(const struct Rect *r);

_returns_(OK) int rect_from_center(
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

_returns_(okay) int mysincos(double value, _out_ double *s, _out_ double *c) {
	*s= sin(value);
	*c= cos(value);
	return okay;
}

#endif

