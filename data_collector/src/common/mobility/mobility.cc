#include <omnetpp.h>
#include "mobility.h"

using namespace omnetpp;

Define_Module(Mobility);

void Mobility::initialize()
{    Object::initialize();
double speed = par("initialSpeed");
double heading = par("initialHeading");

maxRange = par("maxRange");

moveInterval = par("moveInterval");
moveTimer = new cMessage("move");
divertedMoveTimer = 0;

horScalar = Point(speed * moveInterval, 0);
verScalar = Point(0, speed * moveInterval);

    scheduleAt(0, moveTimer);
}

void Mobility::handleMessage(cMessage *msg)
{
    if (msg == moveTimer)
        moveForward();
}

void Mobility::moveForward()
{
Point velocity;
double ratio = 1;

    if (divertedMoveTimer >= 0)
    {
    double relativeRatio;

        if (divertedMoveTimer <= moveInterval + 1e-9)
        {
            ratio = divertedMoveTimer / moveInterval;
            divertedMoveTimer = -1;
        } else
            divertedMoveTimer -= moveInterval;

        velocity = verScalar * ratio;
        if ((relativeRatio = wrapIfOutside(velocity)) < 1)
        {
            ratio = ratio * relativeRatio;
            ++ endType;
            divertedMoveTimer = -1;
        }
    } else
    {
        velocity = horScalar;
        if ((ratio = wrapIfOutside(velocity)) < 1)
        {
            divertedMoveTimer = fabs(
                    (1.5 * maxRange) / (horScalar.x / moveInterval));

            horScalar.x = -horScalar.x;
        }
    }

    if(endType == 2) return;
    scheduleAfter(moveInterval * ratio, moveTimer);
}
