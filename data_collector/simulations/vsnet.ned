package data_collector.simulations;
import data_collector.dedicated.collector.Collector;
import data_collector.dedicated.sensor.Sensor;

network vsnet
{   
    parameters:
        *.initialX = 10m;
        *.initialY = 10m;
        @display("bgb=500,500");
        
    submodules:
		sensor: Sensor;
		collector: Collector;
}