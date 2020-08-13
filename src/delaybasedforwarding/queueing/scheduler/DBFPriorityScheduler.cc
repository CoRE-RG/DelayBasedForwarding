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

#include <list>
#include "delaybasedforwarding/networklayer/ipv4/DBFIpv4HeaderOptions_m.h"
#include "DBFPriorityScheduler.h"
#include "inet/networklayer/ipv4/Ipv4Header_m.h"
#include "delaybasedforwarding/utilities/HelperFunctions.h"
#include "inet/linklayer/ethernet/EtherMacBase.h"
//TODO remove timetag include and creation time tag dependent uses
#include "inet/common/TimeTag_m.h"

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
        cModule *macModule = getParentModule()->getParentModule()->getParentModule()->getSubmodule("mac");
        if ((etherMacFullDuplex = dynamic_cast<inet::EtherMacFullDuplex*>(macModule))) {
            etherMacFullDuplex->subscribe("transmissionStateChanged", this);
        } else {
            throw cRuntimeError("Not the expected EtherMacFullDuplex module: %s",macModule);
        }
        txIdleCounter = 0;
    }
}

void DBFPriorityScheduler::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        int collectionsIdx = selfMsg->getCollectionIdx();
        inet::Packet *scheduledPacket = selfMsg->getScheduledPacket();
        selfMsg->setScheduledPacket(nullptr);
        delete msg;
        msg = nullptr;
        selfMsg = nullptr;
        collections[collectionsIdx]->removePacket(scheduledPacket);
        take(scheduledPacket);
        if (auto dbfHeaderTag = scheduledPacket->findTag<DBFHeaderTag>()) {
            auto ipv4Header = scheduledPacket->popAtFront<inet::Ipv4Header>();
            scheduledPacket->trimFront();

            auto dbfIpv4Header = inet::IntrusivePtr<inet::Ipv4Header>(ipv4Header->dup());
            const inet::TlvOptionBase *tlvOptionBase = dbfIpv4Header->findOptionByType(DBFIpv4OptionType::DBFPARAMETERS);
            DBFIpv4Option *dbfIpv4Option = dynamic_cast<DBFIpv4Option*>(tlvOptionBase->dup());

            inet::TlvOptions &tlvOptions = dbfIpv4Header->getOptionsForUpdate();
            tlvOptions.deleteOptionByType(DBFIpv4OptionType::DBFPARAMETERS, false);

            //########################Remove from here ... ###################################
            auto data = scheduledPacket->peekData();
            auto regions = data->getAllTags<inet::CreationTimeTag>();
            const SimTime simtime = inet::SimTime::parse("246.857090645169s");
            for (auto &region : regions) {
                if (region.getTag()->getCreationTime() == simtime) {
                    EV_DEBUG << "Problematic packet leaving from " << this->getFullName() << endl;
                }
            }
            //########################Remove till here ... ###################################

            dbfIpv4Option->setEDelay(dbfHeaderTag->getEDelay() + simTime() - dbfHeaderTag->getTRcv());
            dbfIpv4Header->addOption(dbfIpv4Option);

            scheduledPacket->insertAtFront(dbfIpv4Header);
            scheduledPacket->removeTag<DBFHeaderTag>();
        }
        if (txIdleCounter == 1 ) {
            txIdleCounter--;
            send(scheduledPacket, "out");
        } else {
            throw cRuntimeError("Value of txIdleCounter is higher than zero: %d", txIdleCounter);
        }
    }
    delete msg;
}

void DBFPriorityScheduler::handleCanPopPacket(cGate *gate)
{
    Enter_Method("DBFPriorityScheduler::handleCanPopPacket");
    lookForExpiredPackets();
    if (txIdleCounter == 1) {
        checkQueues();
    }
}

void DBFPriorityScheduler::checkQueues() {
    Enter_Method("DBFPriorityScheduler::checkQueues");
    int collectionIdx = schedulePacket();
    if (collectionIdx >= 0) {
        inet::Packet *packet = collections[collectionIdx]->getPacket(FRONTIDX);
        if (!selfMsg || packet != selfMsg->getScheduledPacket()) {
            if (selfMsg) {
                cancelAndDelete(selfMsg);
            }
            selfMsg = new DBFScheduleMsg();
            selfMsg->setCollectionIdx(collectionIdx);
            selfMsg->setScheduledPacket(packet);
            if (auto dbfHeaderTag = packet->findTag<DBFHeaderTag>()) {
                simtime_t scheduleTime = SimTime(0.0);
                if (dbfHeaderTag->getTMin() >= simTime()) {
                    scheduleTime = dbfHeaderTag->getTMin();
                } else {
                    scheduleTime = simTime();
                }
                scheduleAt(scheduleTime, selfMsg);
            } else {
                scheduleAt(simTime(), selfMsg);
            }
        }
    }
}

bool DBFPriorityScheduler::isExpired(DBFHeaderTag *dbfHeaderTag) {
    return dbfHeaderTag->getTMax() < simTime();
}

void DBFPriorityScheduler::lookForExpiredPackets() {
   int collectionIdx = schedulePacket();
   auto collection = collectionIdx >= 0 ? collections[collectionIdx] : nullptr;
   int sumOfPackets = collection ? collection->getNumPackets() : 0;
   std::list<inet::Packet*> expiredPackets;
   for (int i = 0; i < sumOfPackets; i++) {
       inet::Packet *packet = collection->getPacket(i);
       if (auto dbfHeaderTag = packet->findTag<DBFHeaderTag>()) {
           if (isExpired(dbfHeaderTag)) {
               expiredPackets.push_back(packet);
           }
           if (selfMsg && selfMsg->getScheduledPacket() == packet) {
               selfMsg->setScheduledPacket(nullptr);
               cancelAndDelete(selfMsg);
               selfMsg = nullptr;
           }
       }
   }

   for (inet::Packet *packet : expiredPackets) {
       collection->removePacket(packet);
       take(packet);
       delete packet;
   }
}

void DBFPriorityScheduler::receiveSignal(cComponent *source, simsignal_t signalID, long l, cObject *details) {
    if (l == inet::EtherMacBase::MacTransmitState::TX_IDLE_STATE) {
        txIdleCounter++;
        if (txIdleCounter == 1) {
            lookForExpiredPackets();
            checkQueues();
        } else {
            throw cRuntimeError("Value of txIdleCounter is higher than zero: %d", txIdleCounter);
        }
    }
}

} //namespace
