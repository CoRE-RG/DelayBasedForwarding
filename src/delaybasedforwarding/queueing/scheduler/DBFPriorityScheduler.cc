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

#define FRONTIDX 0

Define_Module(DBFPriorityScheduler);

DBFPriorityScheduler::~DBFPriorityScheduler() {
    cancelAndDelete(selfMsg);
}

void DBFPriorityScheduler::initialize(int stage) {
    PriorityScheduler::initialize(stage);
    if (stage == inet::INITSTAGE_LOCAL) {
        selfMsg = nullptr;
        enqueuedMsgs = 0;
        currentCollectionsIdx = -1;
        currentScheduledPacket = nullptr;
    }
}

void DBFPriorityScheduler::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        collections[currentCollectionsIdx]->removePacket(currentScheduledPacket);
        currentCollectionsIdx = -1;
        take(currentScheduledPacket);
        currentScheduledPacket->removeTag<DBFHeaderTag>();
        send(currentScheduledPacket, "out");
        currentScheduledPacket = nullptr;
        enqueuedMsgs--;
        if (enqueuedMsgs) {
            checkQueues();
        }
    }
    if (msg != selfMsg) {
        delete msg;
    }
}

void DBFPriorityScheduler::handleCanPopPacket(cGate *gate)
{
    Enter_Method("DBFPriorityScheduler::handleCanPopPacket");
    enqueuedMsgs++;
    checkQueues();
}

void DBFPriorityScheduler::checkQueues() {
    int collectionIdx = schedulePacket();
    if (collectionIdx >= 0) {
        inet::Packet *packet = collections[collectionIdx]->getPacket(FRONTIDX);
        if (packet != currentScheduledPacket) {
            currentScheduledPacket = packet;
            currentCollectionsIdx = collectionIdx;
            if (selfMsg) {
                cancelAndDelete(selfMsg);
            }
            selfMsg = new cMessage();
            if (auto dbfHeaderTag = packet->findTag<DBFHeaderTag>()) {
                if (isExpired(dbfHeaderTag)) {
                    collections[currentCollectionsIdx]->removePacket(currentScheduledPacket);
                    currentCollectionsIdx = -1;
                    take(currentScheduledPacket);
                    currentScheduledPacket = nullptr;
                    enqueuedMsgs--;
                    delete selfMsg;
                } else {
                    simtime_t scheduleTime = SimTime(0.0);
                    if (dbfHeaderTag->getTMin() >= simTime()) {
                        scheduleTime = dbfHeaderTag->getTMin();
                    } else {
                        scheduleTime = dbfHeaderTag->getTMax();
                    }
                    scheduleAt(scheduleTime, selfMsg);
                }
            } else {
                scheduleAt(simTime(), selfMsg);
            }
        }
    }
}

bool DBFPriorityScheduler::isExpired(DBFHeaderTag *dbfHeaderTag) {
    return dbfHeaderTag->getTMax() < simTime();
}

} //namespace
