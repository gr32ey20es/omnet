#include "node/communication/routing/networkFormation/NetworkFormation.h"

Define_Module (NetworkFormation);

void
NetworkFormation::startup (void)
{
  unsigned int i;
  cModule *appModule, *nodeModule;

  nodeModule = getParentModule ()->getParentModule ();
  appModule  = nodeModule->getSubmodule ("Application");

  if (appModule->hasPar ("isSink"))
    isSink = appModule->par ("isSink");
  else
    throw cRuntimeError (
        "\nNetworkFormation has to be used with an application that defines the parameter isSink");

  level = 0;
  clusterHead = -1;
  cHeadSetupSeqNum = 0;
  cTopoSetupSeqNum = 0;

  hopLimit = (unsigned int) par ("hopLimit");
  sequenceNumberLimit = (unsigned int) par ("sequenceNumberLimit");

  for (i = 0; i < 101; ++i)
    worker[i] = false;

  if (isSink)
    sendClusterHeadSetupPacket ();
}

void
NetworkFormation::finish (void)
{
  trace () << "self: " << self
           << ", isSink: " << isSink
           << ", clusterHead: " << clusterHead
           << ", transitionNode: " << transitionNode;

  if (self == clusterHead)
    {
      trace () << "\n[";
      for (unsigned int i = 0; i < 101; ++i)
        if (worker[i] == true)
          trace () << i << ", ";
      trace () << "]";
    }
}

void
NetworkFormation::sendClusterHeadSetupPacket (void)
{
  NetworkFormationPacket *setupPkt;
  unsigned int *clusterHeads, i, n;

  setupPkt = new NetworkFormationPacket (
      "NetworkFormation's Cluster Head Setup Packet", NETWORK_LAYER_PACKET);

  setupPkt->setSource (SELF_NETWORK_ADDRESS);
  setupPkt->setDestination (BROADCAST_NETWORK_ADDRESS);
  setupPkt->setSequenceNumber (++cHeadSetupSeqNum);

  // Cluster Head(s) setup
  setupPkt->setNetworkFormationPacketKind (
      NETFORMATION_CLUSTER_HEAD_SETUP_PACKET);
  setupPkt->setNetworkFormationPacketHops (hopLimit);
  setupPkt->setSourceNum (self);

  // Cluster Head(s)
  clusterHeads = new unsigned int[] {22, 27, 72, 77};
  n = 4;

  setupPkt->setClusterHeadsArraySize (n);
  for (i = 0; i < n; ++i)
    setupPkt->setClusterHeads (i, clusterHeads[i]);

  toMacLayer (setupPkt, BROADCAST_MAC_ADDRESS);
}

void
NetworkFormation::sendClusterTopoSetupPacket (void)
{
  if (clusterHead < 0)
    return;

  cTopoSetupSeqNum = cHeadSetupSeqNum;

  NetworkFormationPacket *setupPkt;

  setupPkt = new NetworkFormationPacket (
      "NetworkFormation's Cluster Topo Setup Packet", NETWORK_LAYER_PACKET);

  setupPkt->setSource (SELF_NETWORK_ADDRESS);
  setupPkt->setDestination (BROADCAST_NETWORK_ADDRESS);
  setupPkt->setSequenceNumber (cTopoSetupSeqNum);

  // Cluster Head(s) setup
  setupPkt->setNetworkFormationPacketKind (
      NETFORMATION_CLUSTER_TOPO_SETUP_PACKET);
  setupPkt->setNetworkFormationPacketHops (hopLimit);
  setupPkt->setSourceNum (self);

  // Cluster Head
  setupPkt->setClusterHeadsArraySize (1);
  setupPkt->setClusterHeads (0, clusterHead);

  toMacLayer (setupPkt, BROADCAST_MAC_ADDRESS);
}

void
NetworkFormation::sendClusterJoiningPacket (void)
{
  if (clusterHead < 0)
    return;

  NetworkFormationPacket *setupPkt;

  setupPkt = new NetworkFormationPacket (
      "NetworkFormation's Cluster Joining Packet", NETWORK_LAYER_PACKET);

  setupPkt->setSource (SELF_NETWORK_ADDRESS);
  setupPkt->setDestination (BROADCAST_NETWORK_ADDRESS);

  setupPkt->setNetworkFormationPacketKind (
      NETFORMATION_CLUSTER_JOINING_PACKET);
  setupPkt->setNetworkFormationPacketHops (hopLimit);
  setupPkt->setSourceNum (self);
  setupPkt->setDestinationNum (transitionNode);

  toMacLayer (setupPkt, BROADCAST_MAC_ADDRESS);
}

void
NetworkFormation::fromApplicationLayer (cPacket* pkt, const char *destination)
{}

void
NetworkFormation::fromMacLayer (cPacket* pkt, int srcMacAddress,
                                double rssi, double lqi)
{
  // Base Station does not enjoy this party
  if (isSink)
    return;

  unsigned int packetHops;
  NetworkFormationPacket *netPacket;

  netPacket = dynamic_cast <NetworkFormationPacket*> (pkt);
  packetHops = netPacket->getNetworkFormationPacketHops ();

  // Dropping packet if hops equals 0
  if (!netPacket || packetHops == 0)
    return;

  switch (netPacket->getNetworkFormationPacketKind ())
    {
      case NETFORMATION_CLUSTER_HEAD_SETUP_PACKET:
        handleClusterHeadSetupPacket (netPacket);
        break;

      case NETFORMATION_CLUSTER_TOPO_SETUP_PACKET:
        handleClusterTopoSetupPacket (netPacket);
        break;

      case NETFORMATION_CLUSTER_JOINING_PACKET:
        handleClusterJoiningPacket (netPacket);
        break;
    }
}

void
NetworkFormation::handleClusterHeadSetupPacket (
    NetworkFormationPacket *cHeadSetupPkt)
{
  bool isClusterHead, isSendedCTopoSetup;
  unsigned int seqNum, packetHops, i, n;
  NetworkFormationPacket *dupPacket;

  seqNum = cHeadSetupPkt->getSequenceNumber ();
  // Dropping packet if it was received
  if (seqNum <= cHeadSetupSeqNum)
    return;

  cHeadSetupSeqNum = seqNum;
  packetHops = cHeadSetupPkt->getNetworkFormationPacketHops ();

  // Forwarding
  dupPacket = cHeadSetupPkt->dup ();
  dupPacket->setSource (SELF_NETWORK_ADDRESS);
  dupPacket->setNetworkFormationPacketHops (packetHops - 1);

  toMacLayer (dupPacket, BROADCAST_MAC_ADDRESS);

  // Sending cluster topo setup packet if node is cluster head
  isClusterHead = false;
  n = cHeadSetupPkt->getClusterHeadsArraySize ();

  for (i = 0; i < n; ++i)
    if (cHeadSetupPkt->getClusterHeads (i) == self)
      {
        isClusterHead = true;
        break;
      }

  if (isClusterHead)
    {
      isSendedCTopoSetup = clusterHead == self;
      clusterHead = self;
      level = hopLimit;

      if (!isSendedCTopoSetup || cTopoSetupSeqNum != cHeadSetupSeqNum)
        sendClusterTopoSetupPacket ();
    }
}

void
NetworkFormation::handleClusterTopoSetupPacket (
    NetworkFormationPacket *cTopoSetupPkt)
{
  unsigned int seqNum, packetHops;
  NetworkFormationPacket *dupPacket;

  seqNum = cTopoSetupPkt->getSequenceNumber ();
  // Dropping old packet
  if (seqNum < cTopoSetupSeqNum)
    return;

  cTopoSetupSeqNum = seqNum;
  packetHops = cTopoSetupPkt->getNetworkFormationPacketHops ();

  // Dropping if it was received and CH is not nearer than
  if (!(clusterHead == -1 || level <= packetHops))
    return;

  level = packetHops;
  clusterHead = cTopoSetupPkt->getClusterHeads (0);
  transitionNode = cTopoSetupPkt->getSourceNum ();

  // Forwarding
  dupPacket = cTopoSetupPkt->dup ();
  dupPacket->setSource (SELF_NETWORK_ADDRESS);
  dupPacket->setSourceNum (self);
  dupPacket->setNetworkFormationPacketHops (packetHops - 1);

  toMacLayer (dupPacket, BROADCAST_MAC_ADDRESS);

  // Sending Joining Packet
  sendClusterJoiningPacket();
}

void
NetworkFormation::handleClusterJoiningPacket (
    NetworkFormationPacket *cJoiningPkt)
{
  unsigned int transitionNodePkt, sourceNum;
  NetworkFormationPacket *dupPacket;

  sourceNum = cJoiningPkt->getSourceNum ();
  transitionNodePkt = cJoiningPkt->getDestinationNum ();

  if (self == transitionNodePkt)
    {
      if (self == clusterHead)
          worker[sourceNum] = true;
      else
        {
          dupPacket = cJoiningPkt->dup ();
          dupPacket->setDestinationNum (transitionNode);
          toMacLayer (dupPacket, BROADCAST_MAC_ADDRESS);
        }
    }
}
