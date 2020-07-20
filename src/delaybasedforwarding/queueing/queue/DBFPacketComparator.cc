/*
 * DBFPacketComparator.cc
 *
 *  Created on: Jul 19, 2020
 *      Author: mehkir
 */

#include <delaybasedforwarding/queueing/queue/DBFPacketComparator.h>
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeader_m.h"
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeaderTag_m.h"

namespace delaybasedforwarding {

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
        if ((tag1->getAdmit() && tag2->getAdmit()) || (!tag1->getAdmit() && !tag2->getAdmit())) {
            result = -1*(int)(tag1->getDMin() - tag2->getDMin()).dbl();
        } else if (tag1->getAdmit()) {
            result = tag1->getAdmit();
        } else {
            result = -1*tag2->getAdmit();
        }
    } else if (tag1) {
        result += 1;
    } else if (tag2) {
        result -= 1;
    }
    return result;
}

DBFPacketComparator* DBFPacketComparator::dup() const {
    return new DBFPacketComparator();
}

} /* namespace delaybasedforwarding */
