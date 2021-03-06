#include "mytest.h"

#include <stdio.h>
#include <string.h>

int print_string(const char *s) {
	printf("'%s'\n", s);
	return strlen(s);
}

void print_string_fix(const char *s, int l) {
	printf("%d characters in '%s'\n", l, s);
}

void get_name(char *s, int l) {
	const char name[] = "Guido";
	strncpy(s, name, l);
}

float area(const struct Rect *r) {
    return (r->a.x - r->b.x) * (r->a.y - r->b.y);
}

int rect_from_center(struct Rect *r, const struct Point *c, const float w, const float h) {
    r->a.x = c->x - w / 2;
    r->b.x = c->x + w / 2;
    r->a.y = c->y - h / 2;
    r->b.y = c->y + h / 2;
	return 42;
}

int multiply(const int w, const int h, float *result, const float *a, const float *b) {
	for (int i = 0; i < w * h; ++i) result[i] = a[i] * b[i];
	return 0;
}

_returns_(okay) int mysincos(double value, _out_ double *s, _out_ double *c) {
	*s = sin(value);
	*c = cos(value);
	return okay;
}

