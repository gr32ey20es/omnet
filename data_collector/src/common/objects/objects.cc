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

    location = Point(x, y);
    boundRect = Rectangle(minX, minY, maxX - minX, maxY - minY);

    displayString();
}
void Object::displayString() const
{
cDisplayString& displayStr = getDisplayString();

    displayStr.setTagArg("p", 0, location.x);
    displayStr.setTagArg("p", 1, location.y);
}


const Point& Object::getLocation() const
{
    return location;
}
const Rectangle& Object::getBoundRect() const
{
    return boundRect;
}


double Object::distanceTo(const Point& p) const
{
    return location.distanceTo(p);
}
double Object::wrapIfOutside(const Point& vector)
{
double vectorRatio = 1;
double delta;

    if(vector.x != 0)
    {
        if(vector.x < 0)
        {
            delta = location.x - boundRect.x;
            vectorRatio = std::min(delta / abs(vector.x), vectorRatio);
        } else
        {
            delta = (boundRect.x + boundRect.width) - location.x;
            vectorRatio = std::min(delta / vector.x, vectorRatio);
        }
    }

    if(vector.y != 0)
    {
        if(vector.y < 0)
        {
            delta = location.y - boundRect.y;
            vectorRatio = std::min(delta / abs(vector.y), vectorRatio);
        } else
        {
            delta = (boundRect.y + boundRect.height) - location.y;
            vectorRatio = std::min(delta / vector.y, vectorRatio);
        }
    }

    location = location + vector * vectorRatio;

    displayString();
    return vectorRatio;
}
