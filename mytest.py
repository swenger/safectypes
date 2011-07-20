import numpy
import safectypes

lib = safectypes.load_dll("mytest.so", "mytest.h")

p1 = lib.Point(1.0, 2.0)
p2 = lib.Point(4.0, 1.0)
r1 = lib.Rect(p1, p2)
f1 = lib.area(r1)
print f1, (r1.a.x - r1.b.x) * (r1.a.y - r1.b.y);

p3 = lib.Point(5, 6)
w = 3
h = w
r2 = lib.rect_from_center(p3, h=h)
print (r2.a.x, r2.a.y, r2.b.x, r2.b.y), (p3.x - 0.5 * w, p3.y - 0.5 * h, p3.x + 0.5 * w, p3.y + 0.5 * h)

a = 2 * numpy.ones((3, 4), dtype=numpy.float32)
b = 3 * numpy.ones((3, 4), dtype=numpy.float32)
c = numpy.zeros((3, 4), dtype=numpy.float32)
lib.multiply(result=c, a=a, b=b)
print c

d = 4 * numpy.ones((3, 4), dtype=numpy.float32)
e = lib.multiply(a, d)
print e

print lib.mysincos(3.14)

