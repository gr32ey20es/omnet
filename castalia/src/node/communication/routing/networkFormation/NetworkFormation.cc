#include "node/communication/routing/networkFormation/NetworkFormation.h"

Define_Module(NetworkFormation);

void NetworkFormation::startup()
{
    cModule *appModule = getParentModule()->getParentModule()->getSubmodule("Application");

    if (appModule->hasPar("isSink")) { isSink = appModule->par("isSink"); }
    else { throw cRuntimeError("\nNetworkFormation has to be used with an application that defines the parameter isSink"); }

    level = 0;
    clusterHead = -1;

    hopLimit = (unsigned int)par("hopLimit");
    sequenceNumber = -1;
    sequenceNumberLimit = (unsigned int)par("sequenceNumberLimit");

    if (isSink) { sendClusterHeadSetupPacket(); }
}

void NetworkFormation::finish()
{
    trace() << "self: " << self << ", isSink: " << isSink << ", clusterHead: " << clusterHead;
}

void NetworkFormation::sendClusterHeadSetupPacket()
{
    NetworkFormationPacket *setupPkt = new NetworkFormationPacket(
            "NetworkFormation's Cluster Head Setup Packet",
            NETWORK_LAYER_PACKET
    );

    setupPkt->setSource(SELF_NETWORK_ADDRESS);
    setupPkt->setDestination(BROADCAST_NETWORK_ADDRESS);
//  Cluster Head(s) setup
    setupPkt->setNetworkFormationPacketKind(NETFORMATION_CLUSTER_HEAD_SETUP_PACKET);
    setupPkt->setNetworkFormationPacketHops(hopLimit);
    setupPkt->setNetworkFormationSequenceNum(++sequenceNumber);
//  Cluster Head(s)
    setupPkt->setClusterHeadsArraySize(4);
    setupPkt->setClusterHeads(0, 22);
    setupPkt->setClusterHeads(1, 72);
    setupPkt->setClusterHeads(2, 77);
    setupPkt->setClusterHeads(3, 27);

    toMacLayer(setupPkt, BROADCAST_MAC_ADDRESS);
}

void NetworkFormation::sendClusterTopoSetupPacket()
{
    if (clusterHead < 0) return;

    NetworkFormationPacket *setupPkt = new NetworkFormationPacket(
            "NetworkFormation's Cluster Topo Setup Packet",
            NETWORK_LAYER_PACKET
    );

    setupPkt->setSource(SELF_NETWORK_ADDRESS);
    setupPkt->setDestination(BROADCAST_NETWORK_ADDRESS);
//  Cluster Head(s) setup
    setupPkt->setNetworkFormationPacketKind(NETFORMATION_CLUSTER_TOPO_SETUP_PACKET);
    setupPkt->setNetworkFormationPacketHops(hopLimit);
    setupPkt->setNetworkFormationSequenceNum(++sequenceNumber);
//  Cluster Head
    setupPkt->setClusterHeadsArraySize(1);
    setupPkt->setClusterHeads(0, clusterHead);

    toMacLayer(setupPkt, BROADCAST_MAC_ADDRESS);
}

void NetworkFormation::fromApplicationLayer(cPacket* pkt, const char *destination) {}

void NetworkFormation::fromMacLayer(cPacket* pkt, int srcMacAddress, double rssi, double lqi)
{
NetworkFormationPacket* netPacket,* dupPacket;
unsigned int packetSequenceNumber, packetHops;
bool isCTopoSPacketSended;

    if (isSink) return;     // Base Station does not enjoy this party

    netPacket = dynamic_cast <NetworkFormationPacket*>(pkt);
    packetHops = netPacket->getNetworkFormationPacketHops();
    if (!netPacket || packetHops == 0)
        return;    // Dropping packet if hops equals 0

    if (self == 0) trace() << netPacket->getNetworkFormationPacketKind();

    switch (netPacket->getNetworkFormationPacketKind()) {

        case NETFORMATION_CLUSTER_HEAD_SETUP_PACKET:
        {
            packetSequenceNumber = netPacket->getNetworkFormationSequenceNum();
            if (sequenceNumber <= packetSequenceNumber)
                return;        // Dropping packet if it received

            sequenceNumber = packetSequenceNumber;

            for (unsigned int index = 0; index < netPacket->getClusterHeadsArraySize(); ++index)
                if (netPacket->getClusterHeads(index) == self) {
                    isCTopoSPacketSended = clusterHead == self;
                    clusterHead = self;
                    level = hopLimit;
                }

            if (clusterHead == self && !isCTopoSPacketSended) { sendClusterTopoSetupPacket(); }

            dupPacket = netPacket->dup();
            dupPacket->setSource(SELF_NETWORK_ADDRESS);
            dupPacket->setNetworkFormationPacketHops(packetHops - 1);

            toMacLayer(dupPacket, BROADCAST_MAC_ADDRESS);
            break;
        }

        case NETFORMATION_CLUSTER_TOPO_SETUP_PACKET:
        {
            if (!(clusterHead == -1 || level <= packetHops))
                return;     // Dropping if it received and CH is not nearer than

            clusterHead = netPacket->getClusterHeads(0);
            level = packetHops;

            dupPacket = netPacket->dup();
            dupPacket->setSource(SELF_NETWORK_ADDRESS);
            dupPacket->setNetworkFormationPacketHops(packetHops - 1);

            toMacLayer(dupPacket, BROADCAST_MAC_ADDRESS);
            break;
        }
    }
}
