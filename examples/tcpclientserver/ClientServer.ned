//
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

package delaybasedforwarding.examples.tcpclientserver;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import delaybasedforwarding.node.inet.DBFHost;
import inet.node.ethernet.EtherLink;
import delaybasedforwarding.node.inet.DBFRouter;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;

network ClientServer
{
    parameters:
        double _datarate @unit(bps) = 100Mbps;
        double _length @unit(m) = 100000000.0m;

    types:
        channel ETH_100MBIT extends EtherLink
        {
            datarate = _datarate;
            length = _length;
        }

    submodules:
        client1: DBFHost {
            parameters:
                @display("p=180,77;i=device/pc3");
        }
        client2: DBFHost {
            parameters:
                @display("p=180,150;i=device/pc3");
        }
        client3: DBFHost {
            parameters:
                @display("p=180,228;i=device/pc3");
        }
        server: DBFHost {
            parameters:
                @display("p=631,150;i=device/pc2");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=45,101;is=s");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=45,203");
        }

        router1: DBFRouter {
            @display("p=345,150");
        }
        router2: DBFRouter {
            @display("p=502,151");
        }
    connections:
        client1.ethg++ <--> ETH_100MBIT <--> router1.ethg++;
        client2.ethg++ <--> ETH_100MBIT <--> router1.ethg++;
        client3.ethg++ <--> ETH_100MBIT <--> router1.ethg++;
        router1.ethg++ <--> ETH_100MBIT <--> router2.ethg++;
        server.ethg++ <--> ETH_100MBIT <--> router2.ethg++;
}

