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

namespace delaybasedforwarding {

Define_Module(DBFPriorityClassifier);

void DBFPriorityClassifier::initialize(int stage) {
    inet::queueing::PacketClassifierBase::initialize(stage);
    if (stage == inet::INITSTAGE_LAST) {
        createDBFQueue();
    }
}

void DBFPriorityClassifier::handleMessage(cMessage *msg)
{
    //TODO override classifyPacket to choose an appropriate queue
    auto packet = dynamic_cast<inet::Packet*>(msg);
    int consumerIdx = classifyPacket(packet);
    consumers[consumerIdx]->pushPacket(packet, outputGates[consumerIdx]);
}

void DBFPriorityClassifier::createDBFQueue() {
    // 1. createQueue()
    cModule *dbfQueue = createFinalizeAndScheduleDynamicModule("delaybasedforwarding.queueing.queue.DBFPacketQueue", "queue", this->getParentModule(), true);
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
    numGates = this->gateSize("out");
    auto outputGate = this->gate("out", numGates-1);
    auto queueInGate = dbfQueue->gate("in");
    outputGate->connectTo(queueInGate);
    // 4. pushBackQueue() in classifier
    outputGates.push_back(outputGate);
    consumers.push_back(dynamic_cast<IPassivePacketSink*>(dbfQueue));
    // 5. inform scheduler
    DBFPriorityScheduler *dbfPriorityScheduler = dynamic_cast<DBFPriorityScheduler*>(this->getParentModule()->getSubmodule("scheduler"));
    dbfPriorityScheduler->addDBFQueue(dbfQueue);
}

} //namespace
