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

package delaybasedforwarding.examples.jitter_reduction;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.StandardHost;
import delaybasedforwarding.node.inet.DBFHost;
import inet.node.ethernet.EtherLink;
import inet.node.inet.Router;
import delaybasedforwarding.node.inet.DBFRouter;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.node.inet.INetworkNode;

network JitterReduction
{
    parameters:
        double _datarate @unit(bps) = 100Mbps;
        double _length @unit(m) = 20000.0m;

    types:
        channel ETH_100MBIT extends EtherLink
        {
            datarate = _datarate;
            length = _length;
        }

    submodules:
        S1_R1: DBFHost {
            parameters:
                @display("p=140,160;i=device/pc3");
        }

        S2_R2: StandardHost {
            parameters:
                @display("p=300,25;i=device/pc3");
        }

        S3_R3: StandardHost {
            parameters:
                @display("p=460,25;i=device/pc3");
        }

        S4_R4: StandardHost {
            parameters:
                @display("p=620,25;i=device/pc3");
        }

        S5_R1: StandardHost {
            parameters:
                @display("p=780,25;i=device/pc3");
        }

        FWD1: DBFRouter {
            parameters:
                @display("p=300,160");
        }

        FWD2: DBFRouter {
            parameters:
                @display("p=460,160");
        }

        FWD2_1: Router {
            parameters:
                @display("p=460,295");
        }

        FWD2_2: Router {
            parameters:
                @display("p=460,430");
        }

        FWD3: DBFRouter {
            parameters:
                @display("p=620,160");
        }
        
        FWD3_1: Router {
            parameters:
                @display("p=620,295");
        }
        
        FWD4: DBFRouter {
            parameters:
                @display("p=780,160");
        }

        R1: StandardHost {
            parameters:
                @display("p=940,160;i=device/pc3");
        }

        R2: StandardHost {
            parameters:
                @display("p=460,565;i=device/pc3");
        }
        
        R3: StandardHost {
            parameters:
                @display("p=620,430;i=device/pc3");
        }
        
        R4: StandardHost {
            parameters:
                @display("p=780,295;i=device/pc3");
        }
        
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=45,101;is=s");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=45,203");
        }

    connections:
        S1_R1.ethg++ <--> ETH_100MBIT <--> FWD1.ethg++;
        S2_R2.ethg++ <--> ETH_100MBIT <--> FWD1.ethg++;
        FWD1.ethg++ <--> ETH_100MBIT <--> FWD2.ethg++;

        S3_R3.ethg++ <--> ETH_100MBIT <--> FWD2.ethg++;
        FWD2.ethg++ <--> ETH_100MBIT <--> FWD2_1.ethg++;
        FWD2_1.ethg++ <--> ETH_100MBIT <--> FWD2_2.ethg++;
        FWD2_2.ethg++ <--> ETH_100MBIT <--> R2.ethg++;
        FWD2.ethg++ <--> ETH_100MBIT <--> FWD3.ethg++;

        S4_R4.ethg++ <--> ETH_100MBIT <--> FWD3.ethg++;
        FWD3.ethg++ <--> ETH_100MBIT <--> FWD3_1.ethg++;
        FWD3_1.ethg++ <--> ETH_100MBIT <--> R3.ethg++;
        FWD3.ethg++ <--> ETH_100MBIT <--> FWD4.ethg++;

        S5_R1.ethg++ <--> ETH_100MBIT <--> FWD4.ethg++;
        FWD4.ethg++ <--> ETH_100MBIT <--> R1.ethg++;
        FWD4.ethg++ <--> ETH_100MBIT <--> R4.ethg++;

}
