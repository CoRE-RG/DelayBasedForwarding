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

package delaybasedforwarding.queueing.queue;

import inet.queueing.queue.PacketQueue;

//
// This module implements a packet queue for delay based forwarded packets
//
// @author Mehmet Cakir
//
simple DBFPacketQueue extends PacketQueue
{
    parameters:
        @class(DBFPacketQueue);
        // The custom packet comparator module implementation
        comparatorClass = "delaybasedforwarding::DBFPacketComparator";
        // The custom packet dropper module implementation
        dropperClass = "delaybasedforwarding::DBFPacketDropper";
}
