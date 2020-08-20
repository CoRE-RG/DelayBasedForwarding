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

#include "DBFPriorityClassifier.h"
#include "delaybasedforwarding/utilities/DynamicModuleHandling.h"
#include "delaybasedforwarding/queueing/scheduler/DBFPriorityScheduler.h"
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeaderTag_m.h"

namespace delaybasedforwarding {

Define_Module(DBFPriorityClassifier);

void DBFPriorityClassifier::initialize(int stage) {
    inet::queueing::PacketClassifierBase::initialize(stage);
    if (stage == inet::INITSTAGE_LOCAL) {
        deltaSteps = par("deltaSteps");
        maximumDelta = par("maximumDelta");
        deltaQueueMap = new std::map<simtime_t, int, std::less<simtime_t>>();
    }
}

void DBFPriorityClassifier::handleMessage(cMessage *msg) {
    auto packet = dynamic_cast<inet::Packet*>(msg);
    int consumerIdx = classifyPacket(packet);
    consumers[consumerIdx]->pushPacket(packet, outputGates[consumerIdx]);
}

int DBFPriorityClassifier::classifyPacket(inet::Packet *packet) {
    int consumerIdx = -1;
    if (auto dbfHeaderTag = packet->findTag<DBFHeaderTag>()) {
        simtime_t lqbudget = dbfHeaderTag->getLqBudgetMax() - dbfHeaderTag->getLqBudgetMin();
        if (lqbudget >= maximumDelta) {
            consumerIdx = getIndexOfQueueForDelta(maximumDelta);
        } else {
            simtime_t bestDelta = deltaSteps;
            while(lqbudget > bestDelta) {
                bestDelta += deltaSteps;
            }
            consumerIdx = getIndexOfQueueForDelta(bestDelta);
        }
    } else {
        consumerIdx = getIndexOfQueueForDelta(maximumDelta);
    }
    return consumerIdx;
}

int DBFPriorityClassifier::getIndexOfQueueForDelta(simtime_t delta) {
    int consumerIdx = -1;
    for (auto it = deltaQueueMap->begin(); it != deltaQueueMap->end(); ++it) {
        if (it->first == delta) {
            consumerIdx = it->second;
            break;
        }
    }
    if (consumerIdx == -1) {
        consumerIdx = createDBFQueue();
        deltaQueueMap->insert({delta,consumerIdx});
    }
    return consumerIdx;
}

int DBFPriorityClassifier::createDBFQueue() {
    int consumerIdx = -1;
    // 1. createQueue()
    cModule *dbfQueue = createDynamicModule("delaybasedforwarding.queueing.queue.DBFPacketQueue", "queue", this->getParentModule(), true);
    if (!dbfQueue) {
        throw cRuntimeError("DBFQueue is null.");
    }
    // 2. createOutGate()
    int numGates = this->gateSize("out");
    this->setGateSize("out", numGates+1);
    if ((this->gateSize("out") - numGates) != 1) {
        throw cRuntimeError("Gate not created.");
    }
    // 3. connectOutGate()
    consumerIdx = this->gateSize("out")-1;
    auto outputGate = this->gate("out", consumerIdx);
    auto queueInGate = dbfQueue->gate("in");
    outputGate->connectTo(queueInGate);
    // 4. pushBackQueue() in classifier
    outputGates.push_back(outputGate);
    consumers.push_back(dynamic_cast<IPassivePacketSink*>(dbfQueue));
    // 5. inform scheduler
    DBFPriorityScheduler *dbfPriorityScheduler = dynamic_cast<DBFPriorityScheduler*>(this->getParentModule()->getSubmodule("scheduler"));
    int providerIdx = dbfPriorityScheduler->addDBFQueue(dbfQueue);
    if (consumerIdx != providerIdx) {
        throw cRuntimeError("Queues are in different indices: At classifier: %d, at scheduler: %d",consumerIdx,providerIdx);
    }
    finalizeModuleAndSchedule(dbfQueue);
    return consumerIdx;
}

std::map<simtime_t, int> *DBFPriorityClassifier::getDeltaQueueMap() {
    return deltaQueueMap;
}

} //namespace
