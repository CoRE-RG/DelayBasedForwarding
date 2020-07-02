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
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeader_m.h"
#include "inet/networklayer/ipv4/Ipv4Header_m.h"
#include "delaybasedforwarding/utilities/HelperFunctions.h"

namespace delaybasedforwarding {

Define_Module(EgressForwarder);

void EgressForwarder::initialize()
{
    // TODO - Generated method body
}

void EgressForwarder::handleMessage(cMessage *msg)
{
    if (inet::Packet *packet = dynamic_cast<inet::Packet*>(msg)) {
        if (containsProtocol(packet, &inet::Protocol::ipv4)) {
            auto ipv4Header = packet->popAtFront<inet::Ipv4Header>();
            auto dbfHeader = packet->peekAtFront<DBFHeader>();
            // Do something with DBFHeader
            packet->trimFront();
            packet->insertAtFront(ipv4Header);
        }
    }
    send(msg,"out");
}

void EgressForwarder::handleRegisterProtocol(const inet::Protocol& protocol, cGate *in, inet::ServicePrimitive servicePrimitive) {
    Enter_Method("handleRegisterProtocol");
    registerProtocol(protocol, getParentModule()->gate("out"), servicePrimitive);
}

void EgressForwarder::handleRegisterService(const inet::Protocol& protocol, cGate *out, inet::ServicePrimitive servicePrimitive) {
    Enter_Method("handleRegisterService");
    registerService(protocol, getParentModule()->gate("out"), servicePrimitive);
}

} //namespace
