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

package delaybasedforwarding.examples.metroringtest;

import inet.node.ethernet.EtherLink;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.StandardHost;
import inet.node.inet.Router;

network MetroRingTest
{
    parameters:


    types:
        channel LinkDelay1000us extends EtherLink
        {
            datarate = 100Mbps; // 24us transmission delay
            length = 195200.0m;  // 976us link delay
        }

    submodules:

        SenderHost1: StandardHost {
            parameters:
                @display("p=653,135");
        }

        SenderHost2: StandardHost {
            parameters:
                @display("p=1139,164");
        }

        SenderHost3: StandardHost {
            parameters:
                @display("p=1192,666");
        }

        SenderHost4: StandardHost {
            parameters:
                @display("p=653,743");
        }

        SenderHost5: StandardHost {
            parameters:
                @display("p=219,693");
        }

        SenderHost6: StandardHost {
            parameters:
                @display("p=193,224");
        }

        ReceiverHost1: StandardHost {
            parameters:
                @display("p=791,135");
        }

        ReceiverHost2: StandardHost {
            parameters:
                @display("p=1192,224");
        }

        ReceiverHost3: StandardHost {
            parameters:
                @display("p=1100,726");
        }

        ReceiverHost4: StandardHost {
            parameters:
                @display("p=791,743");
        }

        ReceiverHost5: StandardHost {
            parameters:
                @display("p=321,726");
        }

        ReceiverHost6: StandardHost {
            parameters:
                @display("p=310,174");
        }

        Router1: Router {
            parameters:
                @display("p=713,290");
        }

        Router2: Router {
            parameters:
                @display("p=1045,290");
        }

        Router3: Router {
            parameters:
                @display("p=1045,591");
        }

        Router4: Router {
            parameters:
                @display("p=713,591");
        }

        Router5: Router {
            parameters:
                @display("p=360,591");
        }

        Router6: Router {
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
        Router1.ethg++ <--> LinkDelay1000us <--> Router2.ethg++;
        Router2.ethg++ <--> LinkDelay1000us <--> Router3.ethg++;
        Router3.ethg++ <--> LinkDelay1000us <--> Router4.ethg++;
        Router4.ethg++ <--> LinkDelay1000us <--> Router5.ethg++;
        Router5.ethg++ <--> LinkDelay1000us <--> Router6.ethg++;
        Router6.ethg++ <--> LinkDelay1000us <--> Router1.ethg++;

        SenderHost1.ethg++ <--> LinkDelay1000us <--> Router1.ethg++;
        SenderHost2.ethg++ <--> LinkDelay1000us <--> Router2.ethg++;
        SenderHost3.ethg++ <--> LinkDelay1000us <--> Router3.ethg++;
        SenderHost4.ethg++ <--> LinkDelay1000us <--> Router4.ethg++;
        SenderHost5.ethg++ <--> LinkDelay1000us <--> Router5.ethg++;
        SenderHost6.ethg++ <--> LinkDelay1000us <--> Router6.ethg++;

        ReceiverHost1.ethg++ <--> LinkDelay1000us <--> Router1.ethg++;
        ReceiverHost2.ethg++ <--> LinkDelay1000us <--> Router2.ethg++;
        ReceiverHost3.ethg++ <--> LinkDelay1000us <--> Router3.ethg++;
        ReceiverHost4.ethg++ <--> LinkDelay1000us <--> Router4.ethg++;
        ReceiverHost5.ethg++ <--> LinkDelay1000us <--> Router5.ethg++;
        ReceiverHost6.ethg++ <--> LinkDelay1000us <--> Router6.ethg++;
}
