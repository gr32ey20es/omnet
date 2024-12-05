package data_collector.common.mobility;
import data_collector.common.objects.Object;

simple Mobility extends Object
{
    parameters:
       	double initialSpeed @unit(mps) = default(50mps);
       	double initialHeading @unit(rad) = default(0deg);
       	double moveInterval @unit(s) = default(0.1s);
       	double maxRange @unit(m) = default(60m);
       	@class(Mobility);
}