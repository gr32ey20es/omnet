#include <omnetpp.h>
#include "../../common/mobility/mobility.h"

using namespace omnetpp;

class Collector : public Mobility
{
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage*) override;
};
