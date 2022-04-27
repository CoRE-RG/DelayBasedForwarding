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
#include "DBFDynamicPriorityScheduler.h"
#include "delaybasedforwarding/utilities/HelperFunctions.h"
#include "delaybasedforwarding/queueing/queue/DBFPacketQueue.h"

namespace delaybasedforwarding {

Define_Module(DBFDynamicPriorityScheduler);

void DBFDynamicPriorityScheduler::initialize(int stage) {
    DBFPriorityScheduler::initialize(stage);
    if (stage == inet::INITSTAGE_LOCAL) {
        scheduledPacket = nullptr;
    }

}

void DBFDynamicPriorityScheduler::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage() && msg == selfMsg) {
        int collectionsIdx = selfMsg->getCollectionIdx();
        inet::Packet* scheduledPacket = this->scheduledPacket;
        this->scheduledPacket = nullptr;
        selfMsg = nullptr;
        if (DBFPacketQueue* queue = dynamic_cast<DBFPacketQueue*>(collections[collectionsIdx])) {
            collections[collectionsIdx]->removePacket(scheduledPacket);
            take(scheduledPacket);
            if (auto dbfHeaderTag = scheduledPacket->findTag<DBFHeaderTag>()) {
                inet::IntrusivePtr<inet::Ipv4Header> dbfIpv4Header = getMutableIpv4Header(scheduledPacket);
                DBFIpv4Option *dbfIpv4Option = getMutableDBFIpv4Option(dbfIpv4Header);
                removeDBFIpv4Options(dbfIpv4Header);
                dbfIpv4Option->setEDelay(dbfHeaderTag->getEDelay() + simTime() - dbfHeaderTag->getTRcv());
                dbfIpv4Header->addOption(dbfIpv4Option);
                updateDBFIpv4Header(scheduledPacket, dbfIpv4Header);
                scheduledPacket->removeTag<DBFHeaderTag>();
            }
            if (txIdleCounter == 1 ) {
                txIdleCounter--;
                send(scheduledPacket, "out");
            } else {
                throw cRuntimeError("Value of txIdleCounter is higher than zero: %d", txIdleCounter);
            }
        }
        else {
            throw cRuntimeError("Queue %d is not of type DBFPacketQueue", collectionsIdx);
        }
    }
    delete msg;
}

void DBFDynamicPriorityScheduler::prepareSelfMsg() {
    int collectionIdx = -1;
    inet::Packet *highestPriorityPacket = nullptr;
    auto deltaQueueMap = dbfPriorityClassifier->getDeltaQueueMap();
    // Loop over collection indices in descending priority
    for (auto it = deltaQueueMap->begin(); it != deltaQueueMap->end(); ++it) {
        if (providers[it->second]->canPopSomePacket(inputGates[it->second]->getPathStartGate())) {
            int sumOfPackets = collections[it->second]->getNumPackets();
            // Loop over packets in current collection
            for (int i = 0; i < sumOfPackets; i++) {
                if (isPacketReadyToSend(collections[it->second]->getPacket(i))) {
                    if (highestPriorityPacket) {
                        int newCollectionIdx = it->second;
                        inet::Packet* newPacket = collections[it->second]->getPacket(i);
                        if ((!isDBFPacket(highestPriorityPacket) && isDBFPacket(newPacket)) ||
                             (isDBFPacket(highestPriorityPacket) && isDBFPacket(newPacket) &&
                             calculateDynamicPriority(highestPriorityPacket) < calculateDynamicPriority(newPacket))
                           ) {
                            collectionIdx = newCollectionIdx;
                            highestPriorityPacket = newPacket;
                        }
                    } else {
                        collectionIdx = it->second;
                        highestPriorityPacket = collections[it->second]->getPacket(i);
                    }
                } else {
                    break;
                }
            }
            if (collectionIdx >= 0 && highestPriorityPacket) {
                if (selfMsg) {
                        scheduledPacket = nullptr;
                        cancelAndDelete(selfMsg);
                }
                selfMsg = new DBFPriorityScheduleMsg();
                selfMsg->setCollectionIdx(collectionIdx);
                scheduledPacket = highestPriorityPacket;
                break; // Comment if the highest priority packet should be found across all queues.
            }
        }
    }
}

void DBFDynamicPriorityScheduler::schedule() {
    Enter_Method("DBFPriorityScheduler::schedule");
    prepareSelfMsg();
    if (selfMsg) {
        simtime_t scheduleTime = simTime();
        if (auto dbfHeaderTag = scheduledPacket->findTag<DBFHeaderTag>()) {
            if (dbfHeaderTag->getTMin() >= simTime()) {
                scheduleTime = dbfHeaderTag->getTMin();
            }
        }
        scheduleAt(scheduleTime, selfMsg);
    }
}

void DBFDynamicPriorityScheduler::lookForExpiredPackets() {
    for (auto collection : collections) {
        int sumOfPackets = collection->getNumPackets();
        std::list<inet::Packet*> expiredPackets;
        for (int i = 0; i < sumOfPackets; i++) {
            inet::Packet *packet = collection->getPacket(i);
            if (auto dbfHeaderTag = packet->findTag<DBFHeaderTag>()) {
                if (isExpired(dbfHeaderTag)) {
                    expiredPackets.push_back(packet);
                    if (selfMsg && scheduledPacket == packet) {
                        scheduledPacket = nullptr;
                        cancelAndDelete(selfMsg);
                    }
                }
            }
        }
        for (inet::Packet *packet : expiredPackets) {
            collection->removePacket(packet);
            delete packet;
        }
    }
}

double DBFDynamicPriorityScheduler::calculateDynamicPriority(inet::Packet* packet) {
    double priority = 0.0;
    auto dbfHeaderTag = packet->findTag<DBFHeaderTag>();
    if (dbfHeaderTag && !hasInfiniteDmax(dbfHeaderTag)) {
        simtime_t result = (simTime() - dbfHeaderTag->getTMin()) / (dbfHeaderTag->getTMax() - dbfHeaderTag->getTMin());
        priority = result.dbl();
        priority = priority < 0.0 ? 0.0 : priority;
        priority = priority > 1.0 ? 1.0 : priority;
    }
    return priority;
}

bool DBFDynamicPriorityScheduler::isDBFPacket(inet::Packet* packet) {
    return packet->findTag<DBFHeaderTag>();
}

} //namespace
