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

import inet.common.misc.NetAnimTrace;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import delaybasedforwarding.node.inet.DBFHost;
import ned.DatarateChannel;
import inet.node.ethernet.EtherSwitch;
import inet.node.ethernet.EtherLink;
import delaybasedforwarding.node.inet.DBFRouter;

network ClientServer
{
    parameters:
        double _datarate @unit(bps) = 100Mbps;
        double _length @unit(m) = 5.0m;
        double _delay @unit(s) = replaceUnit(_length / 2e8, "s");
    
    types:
        channel ETH_100MBIT extends EtherLink
        {
            datarate = _datarate;
            length = _length;
        }

    submodules:
        client1: DBFHost {
            parameters:
                @display("p=180,150;i=device/pc3");
        }
        server: DBFHost {
            parameters:
                @display("p=509,150;i=device/pc2");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=100,100;is=s");
        }
        
        router: DBFRouter {
        }

    connections:
        client1.ethg++ <--> ETH_100MBIT <--> router.ethg++;
        server.ethg++ <--> ETH_100MBIT <--> router.ethg++;
}

