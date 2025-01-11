#ifndef _NETWORKFORMATION_H_
#define _NETWORKFORMATION_H_

#include "node/communication/routing/VirtualRouting.h"
#include "node/communication/routing/networkFormation/NetworkFormationPacket_m.h"

using namespace std;

class NetworkFormation: public VirtualRouting {
 private:
    bool isSink;                //is a .ned file parameter of the Application module

    unsigned int level;         //nearest cluster head (= remaining hop);
    unsigned int clusterHead;   //negative means no cluster head
    unsigned int hopLimit;
    unsigned int sequenceNumber;
    unsigned int sequenceNumberLimit;

 protected:
    void startup();
    void finish();

    void sendClusterHeadSetupPacket();
    void sendClusterTopoSetupPacket();

    void fromApplicationLayer(cPacket *, const char *);
    void fromMacLayer(cPacket *, int, double, double);
};

#endif  //NETWORKFORMATIONMODULE
