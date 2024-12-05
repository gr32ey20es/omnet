#include <omnetpp.h>
#include "../objects/objects.h"

using namespace omnetpp;

class Mobility : public Object
{
    private:
        Point horScalar;
        Point verScalar;

    private:
        double moveInterval;
        cMessage* moveTimer;
        double divertedMoveTimer;

    protected:
        double maxRange;
        int endType = 0;

    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage*) override;

    protected:
        virtual void moveForward();
};

