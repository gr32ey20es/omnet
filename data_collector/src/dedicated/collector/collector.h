#include <omnetpp.h>
#include <unordered_map>
#include "../../common/mobility/mobility.h"
#include "../../common/datagram/datagram_m.h"

using namespace omnetpp;

class Collector : public Mobility
{
    private:
        std::unordered_map<int, double> datas;

    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage*) override;
        virtual void displayString() const override;

    protected:
        virtual double calculateMean() const;
};
