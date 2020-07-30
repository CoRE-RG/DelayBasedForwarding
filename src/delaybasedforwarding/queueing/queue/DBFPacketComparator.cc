/*
 * DBFPacketComparator.cc
 *
 *  Created on: Jul 19, 2020
 *      Author: mehkir
 */

#include <delaybasedforwarding/queueing/queue/DBFPacketComparator.h>
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
    int result = 0;
    auto tag1 = packet1->findTag<DBFHeaderTag>();
    auto tag2 = packet2->findTag<DBFHeaderTag>();
    if (tag1 && tag2) {
        if (tag1->getAdmit() == tag2->getAdmit()) {
            if (tag1->getTMin() < tag2->getTMin()) {
                result = PACKET1HIGHERPRIORITY;
            } else if (tag1->getTMin() > tag2->getTMin()) {
                result = PACKET2HIGHERPRIORITY;
            }
        } else if (tag1->getAdmit()) {
            result = PACKET1HIGHERPRIORITY;
        } else {
            result = PACKET2HIGHERPRIORITY;
        }
    } else if (tag1) {
        result = PACKET1HIGHERPRIORITY;
    } else if (tag2) {
        result = PACKET2HIGHERPRIORITY;
    }
    return result;
}

DBFPacketComparator* DBFPacketComparator::dup() const {
    return new DBFPacketComparator();
}

} /* namespace delaybasedforwarding */
