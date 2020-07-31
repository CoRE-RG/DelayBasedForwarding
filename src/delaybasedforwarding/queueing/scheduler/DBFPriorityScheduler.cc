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

//#include <list>
#include "delaybasedforwarding/networklayer/ipv4/DBFIpv4HeaderOptions_m.h"
#include "DBFPriorityScheduler.h"
#include "inet/networklayer/ipv4/Ipv4Header_m.h"
#include "delaybasedforwarding/utilities/HelperFunctions.h"

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
        delete msg;
        msg = nullptr;
        selfMsg = nullptr;
        collections[currentCollectionsIdx]->removePacket(currentScheduledPacket);
        currentCollectionsIdx = -1;
        take(currentScheduledPacket);
        if (auto dbfHeaderTag = currentScheduledPacket->findTag<DBFHeaderTag>()) {
            auto ipv4Header = currentScheduledPacket->popAtFront<inet::Ipv4Header>();
            currentScheduledPacket->trimFront();

            auto dbfIpv4Header = inet::IntrusivePtr<inet::Ipv4Header>(ipv4Header->dup());
            const inet::TlvOptionBase *tlvOptionBase = dbfIpv4Header->findOptionByType(DBFIpv4OptionType::DBFPARAMETERS);
            DBFIpv4Option *dbfIpv4Option = dynamic_cast<DBFIpv4Option*>(tlvOptionBase->dup());

            inet::TlvOptions &tlvOptions = dbfIpv4Header->getOptionsForUpdate();
            tlvOptions.deleteOptionByType(DBFIpv4OptionType::DBFPARAMETERS, false);
            dbfIpv4Option->setEDelay(dbfHeaderTag->getEDelay() + simTime() - dbfHeaderTag->getTRcv());
            dbfIpv4Header->addOption(dbfIpv4Option);

            currentScheduledPacket->insertAtFront(dbfIpv4Header);
            currentScheduledPacket->removeTag<DBFHeaderTag>();
        }
        send(currentScheduledPacket, "out");
        currentScheduledPacket = nullptr;
        enqueuedMsgs--;
        if (enqueuedMsgs) {
            checkQueues();
        }
    }
    delete msg;
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
                    take(currentScheduledPacket);
                    delete currentScheduledPacket;
                    currentScheduledPacket = nullptr;
                    enqueuedMsgs--;
                    delete selfMsg;
                    selfMsg = nullptr;
                    //lookForMoreExpiredPackets();
                    currentCollectionsIdx = -1;
                    checkQueues();
                } else {
                    simtime_t scheduleTime = SimTime(0.0);
                    if (dbfHeaderTag->getTMin() >= simTime()) {
                        scheduleTime = dbfHeaderTag->getTMin();
                    } else {
                        scheduleTime = simTime();
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

//void DBFPriorityScheduler::lookForMoreExpiredPackets() {
//   int sumOfPackets = collections[currentCollectionsIdx]->getNumPackets();
//   std::list<inet::Packet*> expiredPackets;
//   for (int i = 0; i < sumOfPackets; i++) {
//       inet::Packet *packet = collections[currentCollectionsIdx]->getPacket(i);
//       if (auto dbfHeaderTag = packet->findTag<DBFHeaderTag>()) {
//           if (isExpired(dbfHeaderTag)) {
//               expiredPackets.push_back(packet);
//           }
//       }
//   }
//
//   for (inet::Packet *packet : expiredPackets) {
//       collections[currentCollectionsIdx]->removePacket(packet);
//       take(packet);
//       delete packet;
//       enqueuedMsgs--;
//   }
//}

} //namespace
