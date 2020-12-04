/*
 * DBFPacketDropper.cc
 *
 *  Created on: Dec 4, 2020
 *      Author: mehkir
 */

#include <delaybasedforwarding/queueing/queue/DBFPacketDropper.h>
#include "delaybasedforwarding/utilities/HelperFunctions.h"
#include "DBFPacketQueue.h"

namespace delaybasedforwarding {

Register_Class(DBFPacketDropper);

DBFPacketDropper::DBFPacketDropper() {
}

DBFPacketDropper::~DBFPacketDropper() {
}

inet::Packet* DBFPacketDropper::selectPacket(inet::queueing::IPacketCollection *collection) const {
    if (DBFPacketQueue *queue = dynamic_cast<DBFPacketQueue*>(collection)) {
        inet::Packet *packetToDrop = nullptr;
        int sumOfPackets = collection->getNumPackets();
        for (int i = 0; i < sumOfPackets; i++) {
            inet::Packet *packet = collection->getPacket(i);
            if (auto dbfHeaderTag = packet->findTag<DBFHeaderTag>()) {
                if (isExpired(dbfHeaderTag)) {
                    packetToDrop = packet;
                    break;
                }
            }
        }
        if (!packetToDrop) {
            packetToDrop = collection->getPacket(sumOfPackets-1);
        }
        return packetToDrop;
    } else {
        throw cRuntimeError("The given collection is not a DBFPacketQueue");
    }
}

} /* namespace delaybasedforwarding */
