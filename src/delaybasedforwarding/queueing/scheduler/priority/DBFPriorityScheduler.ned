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

package delaybasedforwarding.queueing.scheduler.priority;

import inet.queueing.scheduler.PriorityScheduler;

//
// This module implements a priority scheduler for delay-based forwarding packets.
//
// @author Mehmet Cakir
//
simple DBFPriorityScheduler extends PriorityScheduler
{
    parameters:
        @class(DBFPriorityScheduler);
        
        // Signal emitted when a packet is pushed into or popped out of a queue.
        @signal[queuesInUseCount](type=long);
        // Statistics to record the number of queues currently in use
        @statistic[queuesInUseCount](title="Queues in use count"; unit=s; record=count,stats,histogram?,vector; interpolationmode=none);
}
