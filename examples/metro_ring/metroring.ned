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

        DBFSenderHost1: DBFHost {
            parameters:
                @display("p=646,135");
        }

        DBFSenderHost2: DBFHost {
            parameters:
                @display("p=1134,163");
        }

        DBFSenderHost3: DBFHost {
            parameters:
                @display("p=1197,644");
        }

        DBFSenderHost4: DBFHost {
            parameters:
                @display("p=652,743");
        }

        DBFSenderHost5: DBFHost {
            parameters:
                @display("p=179,644");
        }

        DBFSenderHost6: DBFHost {
            parameters:
                @display("p=203,215");
        }


        DBFReceiverHost1: DBFHost {
            parameters:
                @display("p=796,135");
        }

        DBFReceiverHost2: DBFHost {
            parameters:
                @display("p=1212,236");
        }

        DBFReceiverHost3: DBFHost {
            parameters:
                @display("p=1095,719");
        }

        DBFReceiverHost4: DBFHost {
            parameters:
                @display("p=788,743");
        }

        DBFReceiverHost5: DBFHost {
            parameters:
                @display("p=279,719");
        }

        DBFReceiverHost6: DBFHost {
            parameters:
                @display("p=328,185");
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

        DBFSenderHost1.ethg++ <--> LinkDelay1000us <--> DBFRouter1.ethg++;
        DBFSenderHost2.ethg++ <--> LinkDelay1000us <--> DBFRouter2.ethg++;
        DBFSenderHost3.ethg++ <--> LinkDelay1000us <--> DBFRouter3.ethg++;
        DBFSenderHost4.ethg++ <--> LinkDelay1000us <--> DBFRouter4.ethg++;
        DBFSenderHost5.ethg++ <--> LinkDelay1000us <--> DBFRouter5.ethg++;
        DBFSenderHost6.ethg++ <--> LinkDelay1000us <--> DBFRouter6.ethg++;

        DBFReceiverHost1.ethg++ <--> LinkDelay1000us <--> DBFRouter1.ethg++;
        DBFReceiverHost2.ethg++ <--> LinkDelay1000us <--> DBFRouter2.ethg++;
        DBFReceiverHost3.ethg++ <--> LinkDelay1000us <--> DBFRouter3.ethg++;
        DBFReceiverHost4.ethg++ <--> LinkDelay1000us <--> DBFRouter4.ethg++;
        DBFReceiverHost5.ethg++ <--> LinkDelay1000us <--> DBFRouter5.ethg++;
        DBFReceiverHost6.ethg++ <--> LinkDelay1000us <--> DBFRouter6.ethg++;
}
