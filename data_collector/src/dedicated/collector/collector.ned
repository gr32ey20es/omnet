package data_collector.dedicated.collector;
import data_collector.common.mobility.Mobility;

simple Collector extends Mobility
{
    parameters:
		@class(Collector);
        
    gates:
    	input gateDirectIn @directIn;
}