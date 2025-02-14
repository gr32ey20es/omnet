package node.communication.routing.networkFormation;

simple NetworkFormation like node.communication.routing.iRouting {
  parameters:
	bool collectTraceInfo = default (true);
	int maxNetFrameSize = default (0);					// bytes, 0 means no limit
	int netBufferSize = default (32);					// number of messages
	int netDataFrameOverhead = default (14);			// bytes
	
	// customization
	int hopLimit = default (7);							// 3-bit
	int sequenceNumberLimit = default (4294967295);		// 4-byte
	
 gates:
	output toCommunicationModule;
	output toMacModule;
	input fromCommunicationModule;
	input fromMacModule;
	input fromCommModuleResourceMgr;
}