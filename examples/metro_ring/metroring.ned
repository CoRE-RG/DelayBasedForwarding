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

package delaybasedforwarding.examples.metro_ring;

import inet.node.ethernet.EtherLink;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import delaybasedforwarding.node.inet.DBFHost;
import delaybasedforwarding.node.inet.DBFRouter;

network MetroRing
{
    parameters:


    types:
        channel LinkDelay1000us extends EtherLink
        {
            datarate = 100Mbps; // 24us transmission delay
            length = 195200.0m;  // 976us link delay
        }

    submodules:

        DBFHost1: DBFHost {
            parameters:
                @display("p=713,135");
        }

        DBFHost2: DBFHost {
            parameters:
                @display("p=1173,185");
        }

        DBFHost3: DBFHost {
            parameters:
                @display("p=1173,693");
        }

        DBFHost4: DBFHost {
            parameters:
                @display("p=713,743");
        }

        DBFHost5: DBFHost {
            parameters:
                @display("p=219,693");
        }

        DBFHost6: DBFHost {
            parameters:
                @display("p=219,191");
        }

        DBFRouter1: DBFRouter {
            parameters:
                @display("p=713,290");
        }

        DBFRouter2: DBFRouter {
            parameters:
                @display("p=1045,290");
        }

        DBFRouter3: DBFRouter {
            parameters:
                @display("p=1045,591");
        }

        DBFRouter4: DBFRouter {
            parameters:
                @display("p=713,591");
        }

        DBFRouter5: DBFRouter {
            parameters:
                @display("p=360,591");
        }

        DBFRouter6: DBFRouter {
            parameters:
                @display("p=360,290");
        }

        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=45,17;is=s");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=45,77");
        }

    connections:
        DBFRouter1.ethg++ <--> LinkDelay1000us <--> DBFRouter2.ethg++;
        DBFRouter2.ethg++ <--> LinkDelay1000us <--> DBFRouter3.ethg++;
        DBFRouter3.ethg++ <--> LinkDelay1000us <--> DBFRouter4.ethg++;
        DBFRouter4.ethg++ <--> LinkDelay1000us <--> DBFRouter5.ethg++;
        DBFRouter5.ethg++ <--> LinkDelay1000us <--> DBFRouter6.ethg++;
        DBFRouter6.ethg++ <--> LinkDelay1000us <--> DBFRouter1.ethg++;
        
        DBFHost1.ethg++ <--> LinkDelay1000us <--> DBFRouter1.ethg++;
        DBFHost2.ethg++ <--> LinkDelay1000us <--> DBFRouter2.ethg++;
        DBFHost3.ethg++ <--> LinkDelay1000us <--> DBFRouter3.ethg++;
        DBFHost4.ethg++ <--> LinkDelay1000us <--> DBFRouter4.ethg++;
        DBFHost5.ethg++ <--> LinkDelay1000us <--> DBFRouter5.ethg++;
        DBFHost6.ethg++ <--> LinkDelay1000us <--> DBFRouter6.ethg++;
}
