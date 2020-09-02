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

#include "IngressForwarder.h"
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeaderTag_m.h"
#include "delaybasedforwarding/utilities/HelperFunctions.h"
#include "inet/linklayer/ethernet/EtherPhyFrame_m.h"
#include "inet/linklayer/ethernet/EtherFrame_m.h"
#include "inet/networklayer/ipv4/Ipv4Header_m.h"
#include "delaybasedforwarding/networklayer/ipv4/DBFIpv4HeaderOptions_m.h"

namespace delaybasedforwarding {

Define_Module(IngressForwarder);

simsignal_t IngressForwarder::rxPkSignal = registerSignal("rxPk");

void IngressForwarder::initialize()
{
}

void IngressForwarder::handleMessage(cMessage *msg)
{
    if (containsIpv4Header(msg) && containsDBFOptions(msg)) {
        attachTrcv(msg);
    }
    send(msg,"out");
}

bool IngressForwarder::containsDBFOptions(cMessage *msg) {
    bool result = false;
    inet::Packet *packet = dynamic_cast<inet::Packet*>(msg);
    auto ipv4Header = packet->peekAtFront<inet::Ipv4Header>();
    const inet::TlvOptionBase *tlvOptionBase = ipv4Header->findOptionByType(DBFIpv4OptionType::DBFPARAMETERS);
    if (tlvOptionBase) {
        auto tlvOptionBaseCastable = tlvOptionBase->dup();
        result = dynamic_cast<DBFIpv4Option*>(tlvOptionBaseCastable) ? true : false;
        delete tlvOptionBaseCastable;
    }
    return result;
}

void IngressForwarder::attachTrcv(cMessage *msg) {
    inet::Packet *packet = dynamic_cast<inet::Packet*>(msg);
    emit(rxPkSignal, packet);
    auto dbfHeaderTag = packet->addTag<DBFHeaderTag>();
    dbfHeaderTag->setTRcv(simTime());
    dbfHeaderTag->setFromNetwork(true);
}

} //namespace
