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
        if (tag1->getTMin() < tag2->getTMin()) {
            result = PACKET1HIGHERPRIORITY;
        } else if (tag1->getTMin() > tag2->getTMin()) {
            result = PACKET2HIGHERPRIORITY;
        } else if (tag1->getAdmit()) {
            result = PACKET1HIGHERPRIORITY;
        } else if (tag2->getAdmit()){
            result = PACKET2HIGHERPRIORITY;
        }
    /*
     * These conditions can only occur for DBF-Packets, which reach the maximum delta.
     * These packets are pushed in the low priority queue with possibly existing BE-Packets.
     * BE-Packets are sent immediately whereas DBF-Packets have to fulfill at least their lqmin.
     */
    } else if (tag1) {
        result = PACKET2HIGHERPRIORITY;
    } else if (tag2 && tag2->getTMin() > omnetpp::simTime()) {
        result = PACKET1HIGHERPRIORITY;
    }
    return result;
}

DBFPacketComparator* DBFPacketComparator::dup() const {
    return new DBFPacketComparator();
}

} /* namespace delaybasedforwarding */
