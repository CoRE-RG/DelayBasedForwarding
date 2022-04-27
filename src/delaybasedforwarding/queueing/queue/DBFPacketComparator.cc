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

//DBF
#include "delaybasedforwarding/queueing/queue/DBFPacketComparator.h"
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeaderTag_m.h"

namespace delaybasedforwarding {

#define PACKET1HIGHERPRIORITY -1
#define PACKET2HIGHERPRIORITY 1

Register_Class(DBFPacketComparator);

DBFPacketComparator::DBFPacketComparator() {
}

DBFPacketComparator::~DBFPacketComparator() {
}

int DBFPacketComparator::comparePackets(inet::Packet *packet1, inet::Packet *packet2) const {
    int result = PACKET2HIGHERPRIORITY;
    auto tag1 = packet1->findTag<DBFHeaderTag>();
    auto tag2 = packet2->findTag<DBFHeaderTag>();
    if (tag1 && tag2) {
        /* send time tMin of packet 1 is earlier than that of packet 2 */
        if (tag1->getTMin() < tag2->getTMin()) {
            result = PACKET1HIGHERPRIORITY;
        /* send time tMin of packet 1 is later than that of packet 2 */
        } else if (tag1->getTMin() > tag2->getTMin()) {
            result = PACKET2HIGHERPRIORITY;
        /* packet 1 and packet 2 have same tMin, packet 1 has the admit bit set, packet 2 not */
        } else if (tag1->getAdmit() && !tag2->getAdmit()) {
            result = PACKET1HIGHERPRIORITY;
        /* otherwise the order remains unchanged */
        //} else {
        //    result = PACKET2HIGHERPRIORITY;
        }
    /*
     * These conditions can occur for DBF-Packets, which reach the maximum delta or in other words have an infinite dMax.
     * These packets are pushed in the low priority queue together with BE-Packets see (@DBFPriorityClassifier::classifyPacket).
     * BE-Packets are sent immediately whereas DBF-Packets have to fulfill at least their tMin.
     */
    // packet 1 is a DBF packet and is already ready to send, packet 2 is a BE packet
    } else if (tag1 && tag1->getTMin() < omnetpp::simTime()) {
        result = PACKET1HIGHERPRIORITY;
    // packet 2 is a DBF packet and is not ready to send, packet 1 is a BE packet
    } else if (tag2 && tag2->getTMin() > omnetpp::simTime()) {
        result = PACKET1HIGHERPRIORITY;
    }
    return result;
}

DBFPacketComparator* DBFPacketComparator::dup() const {
    return new DBFPacketComparator();
}

} /* namespace delaybasedforwarding */
