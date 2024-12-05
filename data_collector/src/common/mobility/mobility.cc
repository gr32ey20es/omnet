#include <omnetpp.h>
#include "mobility.h"

using namespace omnetpp;

Define_Module(Mobility);

void Mobility::initialize()
{
    Object::initialize();

double speed = par("initialSpeed");
double heading = par("initialHeading");
unsigned int moveAnimationFPS = par("moveAnimationFPS");

    speedScalar = Point(speed, 0);
    moveHeading = Transform().rotate(heading);

    moveTimer = new cMessage("moveTimer");
    moveAnimationSPF = 1.0 / moveAnimationFPS;

    scheduleAt(0, moveTimer);
}

void Mobility::handleMessage(cMessage *msg)
{
    if (msg == moveTimer)
        move();
}

void Mobility::move()
{
    this->wrapIfOutside(speedScalar);
    cCanvas* canvas = getSystemModule()->getCanvas();
    cDisplayString& displayStr = getDisplayString();
    const Point& location = this->getLocation();

        displayStr.setTagArg("p", 0, location.x);
        displayStr.setTagArg("p", 1, location.y);
        displayStr.setTagArg("t", 0, location.x);

    scheduleAfter(moveAnimationSPF, moveTimer);
}
