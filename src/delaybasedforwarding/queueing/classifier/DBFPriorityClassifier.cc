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

#include <inet/networklayer/ipv4/Ipv4Header_m.h>

#include "DBFPriorityClassifier.h"
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeader_m.h"
#include "delaybasedforwarding/utilities/HelperFunctions.h"
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeaderTag_m.h"

namespace delaybasedforwarding {

Define_Module(DBFPriorityClassifier);

void DBFPriorityClassifier::handleMessage(cMessage *msg)
{
    //TODO override classifyPacket to choose an appropriate queue
    auto packet = dynamic_cast<inet::Packet*>(msg);
    int consumerIdx = classifyPacket(packet);
    if (containsDBFHeader(msg)) {
        processDBFPacket(msg);
    }
    consumers[consumerIdx]->pushPacket(packet, outputGates[consumerIdx]);
}

bool DBFPriorityClassifier::containsDBFHeader(cMessage *msg) {
    bool containsDBFHeader = false;
    if (inet::Packet *packet = dynamic_cast<inet::Packet*>(msg)) {
        if (containsProtocol(packet, &inet::Protocol::ipv4)) {
            containsDBFHeader = true;
        }
    }
    return containsDBFHeader;
}

void DBFPriorityClassifier::processDBFPacket(cMessage *msg) {
    inet::Packet *packet = dynamic_cast<inet::Packet*>(msg);
    auto ipv4Header = packet->popAtFront<inet::Ipv4Header>();
    auto dbfHeader = packet->peekAtFront<DBFHeader>();
    // Do something with DBFHeader
    auto dbfHeaderTag = packet->addTag<DBFHeaderTag>();
    dbfHeaderTag->setDMin(dbfHeader->getDMin());
    dbfHeaderTag->setDMax(dbfHeader->getDMax());
    dbfHeaderTag->setEDelay(dbfHeader->getEDelay());
    dbfHeaderTag->setAdmit(dbfHeader->getAdmit());
    packet->trimFront();
    packet->insertAtFront(ipv4Header);
}

} //namespace
