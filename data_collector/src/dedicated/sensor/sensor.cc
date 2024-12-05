#include "sensor.h"

Define_Module(Sensor);

void Sensor::initialize()
{
    Object::initialize();

cModule* collModule = getParentModule()->getSubmodule("collector");

id = 10;    // @Override
maxRange = par("maxRange");
sendInterval = par("sendInterval");

collector = check_and_cast<Collector*>(collModule);
collGateDirectIn = collModule->gate("gateDirectIn");
sendTimer = new cMessage();

    scheduleAt(0, this->sendTimer);
}

void Sensor::handleMessage(cMessage* msg)
{
double range;

    if(msg == this->sendTimer)
    {
        range = this->distanceTo(collector->getLocation());
        if(range < this->maxRange)
        {
            generateData();
            sendData();
        }

        scheduleAfter(this->sendInterval, this->sendTimer);
    }
}


void Sensor::generateData()
{
    this->data = uniform(1, 10);
}

void Sensor::sendData()
{
cMessage* packet = new cMessage(
    std::to_string(this->data).c_str()
);

    sendDirect(packet, this->collGateDirectIn);
}
