package data_collector.dedicated.sensor;
import data_collector.common.objects.Object;

simple Sensor extends Object
{
    parameters:
        double maxRange @unit(m) = default(100m);
        double sendInterval @unit(s) = default(1s);
        @class(Sensor);
}