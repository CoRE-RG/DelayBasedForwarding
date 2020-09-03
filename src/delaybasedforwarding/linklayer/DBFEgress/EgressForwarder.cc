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

#include "EgressForwarder.h"
#include "inet/networklayer/ipv4/Ipv4Header_m.h"
#include "inet/transportlayer/udp/UdpHeader_m.h"
#include "inet/transportlayer/tcp_common/TcpHeader_m.h"
#include "delaybasedforwarding/utilities/HelperFunctions.h"

namespace delaybasedforwarding {

Define_Module(EgressForwarder);

simsignal_t EgressForwarder::txPksSignal = registerSignal("txPks");

void EgressForwarder::initialize()
{
}

void EgressForwarder::handleMessage(cMessage *msg)
{
    if (msg->arrivedOn("in")) {
        send(msg,"schedule");
    } else {
        emit(txPksSignal, msg);
        emitPacket(msg);
        send(msg,"out");
    }
}

void EgressForwarder::handleRegisterProtocol(const inet::Protocol& protocol, cGate *in, inet::ServicePrimitive servicePrimitive) {
    Enter_Method("handleRegisterProtocol");
    registerProtocol(protocol, getParentModule()->gate("out"), servicePrimitive);
}

void EgressForwarder::handleRegisterService(const inet::Protocol& protocol, cGate *out, inet::ServicePrimitive servicePrimitive) {
    Enter_Method("handleRegisterService");
    registerService(protocol, getParentModule()->gate("out"), servicePrimitive);
}

void EgressForwarder::emitPacket(cMessage *msg) {
    if (containsIpv4Header(msg)) {
        inet::Packet *packet = dynamic_cast<inet::Packet*>(msg);
        if (!packet) {
            throw cRuntimeError("Given message is not a packet.");
        }
        auto ipv4Header = packet->popAtFront<inet::Ipv4Header>();
        packet->trimFront();
        uint32_t port = -1;
        switch (ipv4Header->getProtocolId()) {
            case inet::IP_PROT_UDP:
                port = packet->peekAtFront<inet::UdpHeader>()->getDestinationPort();
                break;
            case inet::IP_PROT_TCP:
                port = packet->peekAtFront<inet::tcp::TcpHeader>()->getDestinationPort();
                break;
            default:
                throw cRuntimeError("Not supported ProtocolId: %s",ipv4Header->getProtocolId());
                break;
        }
        if (port < 0) {
            throw cRuntimeError("Negative destination port is not allowed: %d",port);
        }
        packet->insertAtFront(ipv4Header);
        auto it = this->txPktToPortSignals.find(port);
        if (it == this->txPktToPortSignals.end()) {
            simsignal_t signaltxPk = registerSignal(("txPkToPort_" + std::to_string(port)).c_str());
            cProperty* statisticTemplate = getProperties()->get("statisticTemplate", "txPk");
            getEnvir()->addResultRecorders(this, signaltxPk, ("txPkToPort_" + std::to_string(port)).c_str(), statisticTemplate);
            txPktToPortSignals.insert({port,signaltxPk});
        }
        emit(this->txPktToPortSignals.at(port), simTime() - msg->getCreationTime());
    }

}

} //namespace
