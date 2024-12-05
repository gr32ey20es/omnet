#include <omnetpp.h>
#include <math.h>
#include "objects.h"

using namespace omnetpp;

Define_Module(Object);

void Object::initialize()
{
double minX = par("minX"), maxX = par("maxX");
double minY = par("minY"), maxY = par("maxY");
double x = par("initialX"), y = par("initialY");

    this->location  = Point(x, y);
    this->boundRect = Rectangle(
            minX, minY,
            maxX - minX, maxY - minY);
}


const Point& Object::getLocation() const
{
    return this->location;
}
const Rectangle& Object::getBoundRect() const
{
    return this->boundRect;
}


double Object::distanceTo(const Point& p) const
{
    return this->location.distanceTo(p);
}
double Object::wrapIfOutside(const Point& vector)
{
double vectorRatio = 1;
double delta;

    if(vector.x != 0)
    {
        delta = location.x - boundRect.x;
        vectorRatio = std::min(delta / vector.x, vectorRatio);

        delta = (boundRect.x + boundRect.width) - location.x;
        vectorRatio = std::min(delta / vector.x, vectorRatio);
    }

    if(vector.y != 0)
    {
        delta = location.y - boundRect.y;
        vectorRatio = std::min(delta / vector.y, vectorRatio);

        delta = (boundRect.y + boundRect.height) - location.y;
        vectorRatio = std::min(delta / vector.y, vectorRatio);
    }

    this->location = this->location + vector * vectorRatio;
    return vectorRatio;
}

