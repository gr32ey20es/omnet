#include "collector.h"

Define_Module(Collector);

void Collector::initialize()
{
    Mobility::initialize();
}
void Collector::handleMessage(cMessage* msg)
{
    if(msg->isSelfMessage())
        Mobility::handleMessage(msg);
    else
    {
        EV << msg;
        delete msg;
    }
}
