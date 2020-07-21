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
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeaderTag_m.h"

namespace delaybasedforwarding {

#define FRONTIDX 0

Define_Module(DBFPriorityScheduler);

DBFPriorityScheduler::~DBFPriorityScheduler() {
    cancelAndDelete(dbfSelfMsg);
    cancelAndDelete(nonDBFSelfMsg);
}

void DBFPriorityScheduler::initialize(int stage) {
    PriorityScheduler::initialize(stage);
    if (stage == inet::INITSTAGE_LOCAL) {
        dbfSelfMsg = nullptr;
        nonDBFSelfMsg = nullptr;
        enqueuedMsgs = 0;
        currentDBFCollectionsIdx = -1;
        currentNONDBFCollectionsIdx = -1;
        currentScheduleTime = SimTime().getMaxTime();
    }
}

void DBFPriorityScheduler::handleMessage(cMessage *msg)
{
    switch (msg->getKind()) {
        case ScheduleKind::DBFPacket: {
            auto packet = collections[currentDBFCollectionsIdx]->getPacket(FRONTIDX);
            if (auto dbfPacket = dynamic_cast<inet::Packet*>(packet)) {
                collections[currentDBFCollectionsIdx]->removePacket(dbfPacket);
                currentScheduleTime = SimTime().getMaxTime();
                enqueuedMsgs--;
                take(dbfPacket);
                send(dbfPacket,"out");
                break;
            }
        }
        case ScheduleKind::NONDBFPacket: {
            auto packet = collections[currentNONDBFCollectionsIdx]->getPacket(FRONTIDX);
            collections[currentNONDBFCollectionsIdx]->removePacket(packet);
            enqueuedMsgs--;
            take(packet);
            send(packet,"out");
            break;
        }
        default:
            throw cRuntimeError("Not known message kind: %d", msg->getKind());
            break;
    }
    if (enqueuedMsgs) {
        checkQueues();
    }
}

void DBFPriorityScheduler::handleCanPopPacket(cGate *gate)
{
    // TODO schedule on lowest tmin and after that look for
    // next packets tmin
    Enter_Method("DBFPriorityScheduler::handleCanPopPacket");
    enqueuedMsgs++;
    checkQueues();
}

void DBFPriorityScheduler::checkQueues() {
    int collectionIdx = schedulePacket();
    if (collectionIdx >= 0) {
        auto packet = collections[collectionIdx]->getPacket(FRONTIDX);
        if (auto dbfHeaderTag = packet->findTag<DBFHeaderTag>()) {
            if (dbfHeaderTag->getDMin() < currentScheduleTime) {
                currentDBFCollectionsIdx = collectionIdx;
                if (dbfSelfMsg) {
                    cancelAndDelete(dbfSelfMsg);
                }
                dbfSelfMsg = new cMessage("selfMsg");
                dbfSelfMsg->setKind(ScheduleKind::DBFPacket);
                currentScheduleTime = dbfHeaderTag->getDMin();
                scheduleAt(simTime() + currentScheduleTime, dbfSelfMsg);
            }
        } else {
            currentNONDBFCollectionsIdx = collectionIdx;
            if (nonDBFSelfMsg) {
                cancelAndDelete(nonDBFSelfMsg);
            }
            nonDBFSelfMsg = new cMessage("nonDBFselfMsg");
            nonDBFSelfMsg->setKind(ScheduleKind::NONDBFPacket);
            scheduleAt(simTime(), nonDBFSelfMsg);
        }
    }
}

} //namespace
