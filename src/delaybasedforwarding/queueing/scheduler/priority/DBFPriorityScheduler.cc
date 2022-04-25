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
#include "delaybasedforwarding/queueing/scheduler/priority/DBFPriorityScheduler.h"
#include "delaybasedforwarding/utilities/HelperFunctions.h"
#include "delaybasedforwarding/queueing/queue/DBFPacketQueue.h"
//INET
#include <inet/linklayer/ethernet/EtherMacBase.h>
//STD
#include <list>

namespace delaybasedforwarding {

#define FRONTIDX 0

Define_Module(DBFPriorityScheduler);

simsignal_t DBFPriorityScheduler::queuesInUseCountSignal = registerSignal("queuesInUseCount");

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
    if (msg->isSelfMessage() && msg == selfMsg) {
        int collectionsIdx = selfMsg->getCollectionIdx();
        selfMsg = nullptr;
        if (DBFPacketQueue* queue = dynamic_cast<DBFPacketQueue*>(collections[collectionsIdx])) {
            inet::Packet *scheduledPacket = queue->popPacket(nullptr);
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

void DBFPriorityScheduler::handleCanPopPacket(cGate *gate) {
    Enter_Method("DBFPriorityScheduler::handleCanPopPacket");
    lookForExpiredPackets();
    if (txIdleCounter == 1) {
        schedule();
    }
}

int DBFPriorityScheduler::determineCollection() {
    int collectionIdx = -1;
    auto deltaQueueMap = dbfPriorityClassifier->getDeltaQueueMap();
    for (auto it = deltaQueueMap->begin(); it != deltaQueueMap->end(); ++it) {
        if (providers[it->second]->canPopSomePacket(inputGates[it->second]->getPathStartGate())) {
            collectionIdx = it->second;
            inet::Packet *packet = collections[it->second]->getPacket(FRONTIDX);
            if (isPacketReadyToSend(packet)) {
                break;
            }
        }
    }
    return collectionIdx;
}

void DBFPriorityScheduler::schedule() {
    Enter_Method("DBFPriorityScheduler::schedule");
    int collectionIdx = determineCollection();
    if (collectionIdx >= 0) {
        inet::Packet *packet = collections[collectionIdx]->getPacket(FRONTIDX);
        if (selfMsg) {
            cancelAndDelete(selfMsg);
        }
        selfMsg = new DBFPriorityScheduleMsg();
        selfMsg->setCollectionIdx(collectionIdx);
        simtime_t scheduleTime = simTime();
        if (auto dbfHeaderTag = packet->findTag<DBFHeaderTag>()) {
            if (dbfHeaderTag->getTMin() >= simTime()) {
                scheduleTime = dbfHeaderTag->getTMin();
            }
        }
        scheduleAt(scheduleTime, selfMsg);
    }
}

void DBFPriorityScheduler::lookForExpiredPackets() {
   for (auto collection : collections) {
       int sumOfPackets = collection->getNumPackets();
       std::list<inet::Packet*> expiredPackets;
       for (int i = 0; i < sumOfPackets; i++) {
           inet::Packet *packet = collection->getPacket(i);
           if (auto dbfHeaderTag = packet->findTag<DBFHeaderTag>()) {
               if (isExpired(dbfHeaderTag)) {
                   expiredPackets.push_back(packet);
                   if (selfMsg && collections[selfMsg->getCollectionIdx()]->getPacket(FRONTIDX) == packet) {
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

void DBFPriorityScheduler::receiveSignal(cComponent *source, simsignal_t signalID, long l, cObject *details) {
    if (dynamic_cast<inet::EtherMacFullDuplex*>(source) && l == inet::EtherMacBase::MacTransmitState::TX_IDLE_STATE) {
        txIdleCounter++;
        if (txIdleCounter == 1) {
            lookForExpiredPackets();
            schedule();
        } else {
            throw cRuntimeError("Value of txIdleCounter is higher than zero: %d", txIdleCounter);
        }
    }
}

void DBFPriorityScheduler::receiveSignal(cComponent *source, simsignal_t signalID, cObject *value, cObject *details) {
    if (dynamic_cast<DBFPacketQueue*>(source)) {
        int queuesInUseCount = 0;
        for (auto collection : collections) {
            if (!(collection->isEmpty())) {
                queuesInUseCount++;
            }
        }
        emit(queuesInUseCountSignal, queuesInUseCount);
    }
}

int DBFPriorityScheduler::addDBFQueue(cModule *dbfQueue) {
    int providerIdx = -1;
    dynamic_cast<inet::queueing::PacketQueue*>(dbfQueue)->subscribe("packetPopped", this);
    dynamic_cast<inet::queueing::PacketQueue*>(dbfQueue)->subscribe("packetRemoved", this);
    dynamic_cast<inet::queueing::PacketQueue*>(dbfQueue)->subscribe("packetPushed", this);
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

bool DBFPriorityScheduler::isPacketReadyToSend(inet::Packet* packet) {
    simtime_t difference = SimTime().ZERO;
    if (auto dbfHeaderTag = packet->findTag<DBFHeaderTag>()) {
        difference = dbfHeaderTag->getTMin() - simTime();
    }
    return difference <= SimTime().ZERO;
}

} //namespace
