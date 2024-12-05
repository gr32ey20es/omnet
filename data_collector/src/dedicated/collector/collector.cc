#include "collector.h"

Define_Module(Collector);

void Collector::initialize()
{    Mobility::initialize();

    displayString();
}
void Collector::handleMessage(cMessage* msg)
{
    if(msg->isSelfMessage())
        Mobility::handleMessage(msg);
    else
    {
    Datagram* packet = check_and_cast<Datagram*>(msg);

        if (datas.count(packet->getSenderID()) == 0)
        {
            datas[packet->getSenderID()] = packet->getData();
            displayString();
        }

        delete msg;
    }
}
void Collector::displayString() const
{   Mobility::displayString();
cDisplayString& displayStr = getDisplayString();

    displayStr.setTagArg("r", 0, maxRange);
    displayStr.setTagArg("t", 0, calculateMean());
}


double Collector::calculateMean() const
{
int numSensor = 0;
double totalValue = 0;

    for (auto& it: datas) {
        ++ numSensor;
        totalValue += it.second;
    }
    return numSensor == 0 ? 0 : totalValue / numSensor;
}
