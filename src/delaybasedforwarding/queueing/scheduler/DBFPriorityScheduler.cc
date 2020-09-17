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
        cModule *macModule = this->getParentModule()->getParentModule()->getParentModule()->getSubmodule("mac");
        if ((etherMacFullDuplex = dynamic_cast<inet::EtherMacFullDuplex*>(macModule))) {
            etherMacFullDuplex->subscribe("transmissionStateChanged", this);
        } else {
            throw cRuntimeError("Not the expected EtherMacFullDuplex module: %s",macModule);
        }
        txIdleCounter = 0;
        cModule *classifierModule = this->getParentModule()->getSubmodule("classifier");
        if((dbfPriorityClassifier = dynamic_cast<DBFPriorityClassifier*>(classifierModule))) {
        } else {
            throw cRuntimeError("Not the expected DBFPriorityClassifier module: %s",classifierModule);
        }
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

int DBFPriorityScheduler::schedulePacket() {
    simtime_t delta = SimTime().ZERO;
    int collectionIdx = -1;
    auto deltaQueueMap = dbfPriorityClassifier->getDeltaQueueMap();
    for (auto it = deltaQueueMap->begin(); it != deltaQueueMap->end(); ++it)
        if (providers[it->second]->canPopSomePacket(inputGates[it->second]->getPathStartGate())) {
            inet::Packet *packet = collections[it->second]->getPacket(FRONTIDX);
            simtime_t difference = SimTime().ZERO;
            if (auto dbfHeaderTag = packet->findTag<DBFHeaderTag>()) {
                difference = dbfHeaderTag->getTMin() - simTime();
            }
            if (delta == SimTime().ZERO || difference < delta ) {
                delta = difference;
                collectionIdx = it->second;
            }
        }
    return collectionIdx;
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

int DBFPriorityScheduler::addDBFQueue(cModule *dbfQueue) {
    int providerIdx = -1;
    // 1. createInGate()
    int numGates = this->gateSize("in");
    this->setGateSize("in", numGates+1);
    if ((this->gateSize("in") - numGates) != 1) {
        throw cRuntimeError("Gate not created.");
    }
    // 2. connectInGate()
    providerIdx = this->gateSize("in")-1;
    auto inputGate = this->gate("in", providerIdx);
    auto queueOutGate = dbfQueue->gate("out");
    queueOutGate->connectTo(inputGate);
    // 3. pushBackQueue() in scheduler
    inputGates.push_back(inputGate);
    providers.push_back(dynamic_cast<IPassivePacketSource*>(dbfQueue));
    collections.push_back(dynamic_cast<IPacketCollection *>(dbfQueue));
    return providerIdx;
}

} //namespace
