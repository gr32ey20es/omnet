#include <omnetpp.h>
#include "geometry.h"

using namespace omnetpp;

bool isGreaterOrEqualPoint(const Point& point, const Point& p)
{
    return (point.x >= p.x && point.y >= p.y);
}

void applyToTransform(Point& p, const Transform& t)
{
double x = p.x;
double y = p.y;

    p.x = t.a * x + t.c * y + t.t1;
    p.y = t.b * x + t.d * y + t.t2;
}

bool isInsideRectangle(const Point& p, const Rectangle& r)
{
double x = p.x;
double y = p.y;
double xMin = r.x;
double yMin = r.y;

    return !(y < yMin || y > yMin + r.height
          || x < xMin || x > xMin + r.width);
}
