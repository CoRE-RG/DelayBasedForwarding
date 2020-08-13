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

#include "DBFPriorityClassifier.h"

namespace delaybasedforwarding {

Define_Module(DBFPriorityClassifier);

void DBFPriorityClassifier::handleMessage(cMessage *msg)
{
    //TODO override classifyPacket to choose an appropriate queue
    auto packet = dynamic_cast<inet::Packet*>(msg);
    int consumerIdx = classifyPacket(packet);
    consumers[consumerIdx]->pushPacket(packet, outputGates[consumerIdx]);
}

} //namespace
