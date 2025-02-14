cplusplus {{
#include "node/communication/routing/RoutingPacket_m.h"
}}

class RoutingPacket;

enum networkFormationPacketDef {
	NETFORMATION_DATA_PACKET = 0;
	NETFORMATION_CLUSTER_HEAD_SETUP_PACKET = 1;
	NETFORMATION_CLUSTER_TOPO_SETUP_PACKET = 2;
} 

packet NetworkFormationPacket extends RoutingPacket {
    // Header
	unsigned int networkFormationPacketKind enum (networkFormationPacketDef);	// 1 byte
	unsigned int networkFormationPacketHops;									// 4 bits
	unsigned int sourceNum;
	// Payload
    unsigned int clusterHeads[];
}

