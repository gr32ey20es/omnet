#include <omnetpp.h>
#include "../objects/objects.h"

using namespace omnetpp;

class Mobility : public Object
{
    private:
        Point speedScalar;
        Transform moveHeading;

    private:
        Point velocityPF;
        cMessage* moveTimer;
        double moveAnimationSPF;

    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;

    protected:
        virtual void move();
};

