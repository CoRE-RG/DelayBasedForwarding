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

#include <cmath>
#include "delaybasedforwarding/utilities/HelperFunctions.h"
#include "delaybasedforwarding/utilities/DynamicModuleHandling.h"
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeaderTag_m.h"
#include "delaybasedforwarding/queueing/classifier/DBFPriorityClassifier.h"
#include "delaybasedforwarding/queueing/scheduler/priority/DBFPriorityScheduler.h"

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
    simtime_t delta = maximumDelta;
    if (auto dbfHeaderTag = packet->findTag<DBFHeaderTag>()) {
        simtime_t lqbudget = SimTime(std::abs(dbfHeaderTag->getLqBudgetMax().dbl() - dbfHeaderTag->getLqBudgetMin().dbl()));
        if (!hasInfiniteDmax(dbfHeaderTag) && lqbudget < maximumDelta) {
            simtime_t bestDelta = deltaSteps;
            while(lqbudget > bestDelta) {
                bestDelta += deltaSteps;
            }
            delta = bestDelta;
        }
    }
    return getIndexOfQueueForDelta(delta);
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
        consumerIdx = createDBFQueue(delta);
        deltaQueueMap->insert({delta,consumerIdx});
    }
    return consumerIdx;
}

int DBFPriorityClassifier::createDBFQueue(simtime_t delta) {
    int consumerIdx = -1;
    // 1. createQueue()
    std::string queueName = "queue(delta=" + delta.str() + ")";
    cModule *dbfQueue = createDynamicModule("delaybasedforwarding.queueing.queue.DBFPacketQueue", queueName.c_str(), this->getParentModule(), true);
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
