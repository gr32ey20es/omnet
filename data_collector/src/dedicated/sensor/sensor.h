#include <omnetpp.h>
#include "../collector/collector.h"
#include "../../common/datagram/datagram_m.h"

using namespace omnetpp;

class Sensor : public Object
{
    private:
        int id;
        double data;
        double maxRange;
        double sendInterval;

    private:
        cMessage* sendTimer;
        Collector* collector;
        cGate* collGateDirectIn;

    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage*) override;
        virtual void displayString() const override;

    protected:
        virtual void sendData();
        virtual void generateData();
};
