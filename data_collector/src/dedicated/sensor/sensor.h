#include <omnetpp.h>
#include "../collector/collector.h"

using namespace omnetpp;

class Sensor : public Object
{
    private:
        int id;
        double data;
        double maxRange;
        double sendInterval;

    private:
        Collector* collector;
        cGate* collGateDirectIn;
        cMessage* sendTimer;

    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage*) override;

    protected:
        virtual void sendData();
        virtual void generateData();
};
