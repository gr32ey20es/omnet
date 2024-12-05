package data_collector.common.mobility;
import data_collector.common.objects.Object;

simple Mobility extends Object
{
    parameters:
       	double initialSpeed @unit(mps) = default(11mps);
       	double initialHeading @unit(rad) = default(0deg);
       	int moveAnimationFPS @unit(fps) = default(1fps);
       	@class(Mobility);
}