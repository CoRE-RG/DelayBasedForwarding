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

#include "Scheduler.h"
#include "inet/common/ModuleAccess.h"

namespace delaybasedforwarding {

Define_Module(Scheduler);

void Scheduler::initialize(int stage)
{
    if (stage == inet::INITSTAGE_LOCAL) {
        enqueueGate = gate("enqueue");
        consumer = inet::findConnectedModule<inet::queueing::IPassivePacketSink>(enqueueGate);
        dequeueGate = gate("dequeue");
        provider = inet::findConnectedModule<inet::queueing::IPassivePacketSource>(dequeueGate);
    }
}

void Scheduler::handleMessage(cMessage *msg)
{
    inet::Packet *packet = dynamic_cast<inet::Packet*>(msg);
    consumer->pushPacket(packet, enqueueGate);
}

inet::queueing::IPassivePacketSink* Scheduler::getConsumer(cGate *gate)
{
    return consumer;
}

void Scheduler::handleCanPushPacket(cGate *gate)
{
    Enter_Method("Scheduler::handleCanPushPacket");
    EV_DEBUG << "Scheduler::handleCanPushPacket" << endl;
}

inet::queueing::IPassivePacketSource* Scheduler::getProvider(cGate *gate)
{
    return provider;
}

void Scheduler::handleCanPopPacket(cGate *gate)
{
    Enter_Method("Scheduler::handleCanPopPacket");
    auto packet = provider->popPacket(dequeueGate);
    take(packet);
    send(packet, "out");
}

} //namespace
