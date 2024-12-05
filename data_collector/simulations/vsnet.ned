package data_collector.simulations;
import data_collector.dedicated.collector.Collector;
import data_collector.dedicated.sensor.Sensor;

network vsnet
{
    parameters:
        **.minX = 0m;
        **.maxX = 200m;
        *.initialX = default(0m);
        **.minY = 0m;
        **.maxY = 200m;
        *.initialY = default(0m);
        @display("bgb=200,200");
        sensors[0].initialX = 50m;
        sensors[0].initialY = 50m;
        sensors[1].initialX = 120m;
        sensors[1].initialY = 50m;
        sensors[2].initialX = 50m;
        sensors[2].initialY = 120m;
        sensors[3].initialX = 120m;
        sensors[3].initialY = 120m;

    submodules:
        sensors[4]: Sensor {
            @display("i=block/broadcast");
        }
        collector: Collector {

            @display("i=block/wheelbarrow");
        }

}
