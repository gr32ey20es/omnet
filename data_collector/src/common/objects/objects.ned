package data_collector.common.objects;

simple Object
{
    parameters:
		double initialX @unit(m);
		double initialY @unit(m);
		double minX @unit(m);
		double minY @unit(m);
		double maxX @unit(m);
		double maxY @unit(m);
		@class(Object);
}