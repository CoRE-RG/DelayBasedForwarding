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

#include "DBFPriorityScheduler.h"

namespace delaybasedforwarding {

Define_Module(DBFPriorityScheduler);

void DBFPriorityScheduler::handleMessage(cMessage *msg)
{
    // Do nothing
}

void DBFPriorityScheduler::handleCanPopPacket(cGate *gate)
{
    Enter_Method("DBFPriorityScheduler::handleCanPopPacket");
    auto packet = collections[0]->getPacket(0);
    collections[0]->removePacket(packet);
    take(packet);
    send(packet,"out");
}

} //namespace
