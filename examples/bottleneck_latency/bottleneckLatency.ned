//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package delaybasedforwarding.examples.bottleneck_latency;

import inet.node.ethernet.EtherLink;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import delaybasedforwarding.node.inet.DBFHost;
import delaybasedforwarding.node.inet.DBFRouter;

network BottleneckLatency
{
    parameters:


    types:
        channel LinkDelay1000us extends EtherLink
        {
            datarate = 100Mbps; // 24us transmission delay
            length = 195200.0m;  // 976us link delay
        }

    submodules:
        Flow_1: DBFHost {
            parameters:
                @display("p=175,73;i=device/pc3");
        }

        Flow_2: DBFHost {
            parameters:
                @display("p=175,160;i=device/pc3");
        }

        Flow_3: DBFHost {
            parameters:
                @display("p=175,256;i=device/pc3");
        }

        Flow_4: DBFHost {
            parameters:
                @display("p=175,345;i=device/pc3");
        }

        FWD1: DBFRouter {
            parameters:
                @display("p=329,73");
        }

        FWD2: DBFRouter {
            parameters:
                @display("p=329,160");
        }

        FWD3: DBFRouter {
            parameters:
                @display("p=329,256");
        }

        FWD4: DBFRouter {
            parameters:
                @display("p=329,345");
        }

        BottleneckLeft: DBFRouter {
            parameters:
                @display("p=459,203");
        }

        BottleneckRight: DBFRouter {
            parameters:
                @display("p=629,203");
        }

        R1: DBFHost {
            parameters:
                @display("p=752,73;i=device/pc3");
        }

        R2: DBFHost {
            parameters:
                @display("p=752,160;i=device/pc3");
        }

        R3: DBFHost {
            parameters:
                @display("p=752,256;i=device/pc3");
        }

        R4: DBFHost {
            parameters:
                @display("p=752,345;i=device/pc3");
        }

        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=45,101;is=s");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=45,203");
        }

    connections:
        Flow_1.ethg++ <--> LinkDelay1000us <--> FWD1.ethg++;
        Flow_2.ethg++ <--> LinkDelay1000us <--> FWD2.ethg++;
        Flow_3.ethg++ <--> LinkDelay1000us <--> FWD3.ethg++;
        Flow_4.ethg++ <--> LinkDelay1000us <--> FWD4.ethg++;

        FWD1.ethg++ <--> LinkDelay1000us <--> BottleneckLeft.ethg++;
        FWD2.ethg++ <--> LinkDelay1000us <--> BottleneckLeft.ethg++;
        FWD3.ethg++ <--> LinkDelay1000us <--> BottleneckLeft.ethg++;
        FWD4.ethg++ <--> LinkDelay1000us <--> BottleneckLeft.ethg++;

        BottleneckLeft.ethg++ <--> LinkDelay1000us <--> BottleneckRight.ethg++;

        BottleneckRight.ethg++ <--> LinkDelay1000us <--> R1.ethg++;
        BottleneckRight.ethg++ <--> LinkDelay1000us <--> R2.ethg++;
        BottleneckRight.ethg++ <--> LinkDelay1000us <--> R3.ethg++;
        BottleneckRight.ethg++ <--> LinkDelay1000us <--> R4.ethg++;
}
