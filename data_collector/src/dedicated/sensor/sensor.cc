#include "sensor.h"

Define_Module(Sensor);

void Sensor::initialize()
{    Object::initialize();
id = getIndex();
maxRange = par("maxRange");
sendInterval = par("sendInterval");

cModule* collModule = getParentModule()->getSubmodule("collector");
sendTimer = new cMessage();
collector = check_and_cast<Collector*>(collModule);
collGateDirectIn = collModule->gate("gateDirectIn");

//    displayString();
    scheduleAt(0, sendTimer);
}
void Sensor::handleMessage(cMessage* msg)
{
    if(msg == sendTimer)
    {
    double range = distanceTo(collector->getLocation());

        if(range < maxRange)
        {
            generateData();
            sendData();
        }

        scheduleAfter(sendInterval, sendTimer);
    }
}
void Sensor::displayString() const
{   Object::displayString();
cDisplayString& displayStr = getDisplayString();

    displayStr.setTagArg("r", 0, maxRange);
}


void Sensor::generateData()
{
    data = id * 10;
}
void Sensor::sendData()
{
Datagram* packet = new Datagram();

    packet->setSenderID(id);
    packet->setData(data);

    sendDirect(packet, collGateDirectIn);
}
