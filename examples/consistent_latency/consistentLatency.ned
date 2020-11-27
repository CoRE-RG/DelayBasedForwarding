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

package delaybasedforwarding.examples.consistent_latency;

import ned.DelayChannel;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.StandardHost;
import delaybasedforwarding.node.inet.DBFHost;
import inet.node.ethernet.EtherLink;
import inet.node.inet.Router;
import delaybasedforwarding.node.inet.DBFRouter;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.node.inet.INetworkNode;

network ConsistentLatency
{
    parameters:
        

    types:
        channel LinkDelay10ms extends EtherLink
        {
            datarate = 1Mbps; // 2,4ms transmission delay
            length = 1520000.0m;  // 7,6ms link delay
        }

        channel LinkDelay100ms extends EtherLink
        {
            datarate = 1Mbps; // 2,4ms transmission delay
            length = 19520000.0m;  // 97,6ms link delay
        }

    submodules:
        S1_R1: DBFHost {
            parameters:
                @display("p=140,160;i=device/pc3");
        }

        S2_R2: DBFHost {
            parameters:
                @display("p=140,310;i=device/pc3");
        }

        FWD1: DBFRouter {
            parameters:
                @display("p=340,160");
        }

        FWD2: DBFRouter {
            parameters:
                @display("p=540,160");
        }

        FWD3: DBFRouter {
            parameters:
                @display("p=240,235");
        }

        FWD4: DBFRouter {
            parameters:
                @display("p=640,235");
        }

        R1: DBFHost {
            parameters:
                @display("p=740,160;i=device/pc3");
        }

        R2: DBFHost {
            parameters:
                @display("p=740,310;i=device/pc3");
        }

        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=45,101;is=s");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=45,203");
        }

    connections:
        S1_R1.ethg++ <--> LinkDelay10ms <--> FWD1.ethg++;
        FWD1.ethg++ <--> LinkDelay10ms <--> FWD2.ethg++;
        FWD2.ethg++ <--> LinkDelay10ms <--> R1.ethg++;

        S2_R2.ethg++ <--> LinkDelay10ms <--> FWD3.ethg++;
        FWD3.ethg++ <--> LinkDelay100ms <--> FWD1.ethg++;
        FWD2.ethg++ <--> LinkDelay100ms <--> FWD4.ethg++;
        FWD4.ethg++ <--> LinkDelay10ms <--> R2.ethg++;

}
