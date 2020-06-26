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

package delaybasedforwarding.linklayer;

import inet.linklayer.contract.ITrafficConditioner;

import delaybasedforwarding.linklayer.DBFEgress.EgressForwarder;
import delaybasedforwarding.linklayer.DBFEgress.Scheduler;
import delaybasedforwarding.linklayer.DBFEgress.Queue;

module DBFEgressTrafficConditioner like ITrafficConditioner
{
    gates:
        input in;
        output out;
    submodules:
        efwd: EgressForwarder {
            @display("p=170,65;i=block/classifier");
        }
        scheduler: Scheduler {
            @display("p=53,213;i=block/timer");
        }
        queue: Queue {
            @display("p=282,213;i=block/queue");
        }
    connections allowunconnected:
        in --> efwd.in;
        out <-- efwd.out;
        //in --> out;
}
