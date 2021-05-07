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

        Host1: StandardHost {
            parameters:
                @display("p=713,135");
        }

        Host2: StandardHost {
            parameters:
                @display("p=1173,185");
        }

        Host3: StandardHost {
            parameters:
                @display("p=1173,693");
        }

        Host4: StandardHost {
            parameters:
                @display("p=713,743");
        }

        Host5: StandardHost {
            parameters:
                @display("p=219,693");
        }

        Host6: StandardHost {
            parameters:
                @display("p=219,191");
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
        
        Host1.ethg++ <--> LinkDelay1000us <--> Router1.ethg++;
        Host2.ethg++ <--> LinkDelay1000us <--> Router2.ethg++;
        Host3.ethg++ <--> LinkDelay1000us <--> Router3.ethg++;
        Host4.ethg++ <--> LinkDelay1000us <--> Router4.ethg++;
        Host5.ethg++ <--> LinkDelay1000us <--> Router5.ethg++;
        Host6.ethg++ <--> LinkDelay1000us <--> Router6.ethg++;
}
