#include "mytest.h"

float area(const struct Rect *r) {
    return (r->a.x - r->b.x) * (r->a.y - r->b.y);
}

void rect_from_center(struct Rect *r, const struct Point *c, const float w, const float h) {
    r->a.x = c->x - w / 2;
    r->b.x = c->x + w / 2;
    r->a.y = c->y - h / 2;
    r->b.y = c->y + h / 2;
}

