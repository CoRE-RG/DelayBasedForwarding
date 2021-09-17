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
