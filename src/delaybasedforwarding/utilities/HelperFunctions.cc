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

#include "HelperFunctions.h"
//INET
#include <inet/networklayer/ipv4/Ipv4Header_m.h>

namespace delaybasedforwarding {

bool containsProtocol(inet::Packet *packet, const inet::Protocol *requestedProtocol) {
    bool containsProtocol = false;
    auto protocolTag = packet->findTag<inet::PacketProtocolTag>();
    if (protocolTag) {
        const inet::Protocol *protocol = protocolTag->getProtocol();
        if (protocol) {
            int ethType = inet::ProtocolGroup::ethertype.findProtocolNumber(protocol);
            containsProtocol = ethType == inet::ProtocolGroup::ethertype.findProtocolNumber(requestedProtocol);
        }
    }
    return containsProtocol;
}

bool containsIpv4Header(omnetpp::cMessage *msg) {
    bool containsIpv4Header = false;
    if (inet::Packet *packet = dynamic_cast<inet::Packet*>(msg)) {
        if (containsProtocol(packet, &inet::Protocol::ipv4)) {
            containsIpv4Header = true;
        }
    }
    return containsIpv4Header;
}

inet::IntrusivePtr<inet::Ipv4Header> getMutableIpv4Header(inet::Packet *packet) {
    inet::Ptr<const inet::Ipv4Header> ipv4Header = packet->peekAtFront<inet::Ipv4Header>();
    return inet::IntrusivePtr<inet::Ipv4Header>(ipv4Header->dup());
}

DBFIpv4Option* getMutableDBFIpv4Option(inet::IntrusivePtr<inet::Ipv4Header> dbfIpv4Header) {
    const inet::TlvOptionBase *tlvOptionBase = dbfIpv4Header->findOptionByType(DBFIpv4OptionType::DBFPARAMETERS);
    return dynamic_cast<DBFIpv4Option*>(tlvOptionBase->dup());
}

void removeDBFIpv4Options(inet::IntrusivePtr<inet::Ipv4Header> dbfIpv4Header) {
    inet::TlvOptions &tlvOptions = dbfIpv4Header->getOptionsForUpdate();
    tlvOptions.deleteOptionByType(DBFIpv4OptionType::DBFPARAMETERS, false);
}

void updateDBFIpv4Header(inet::Packet *packet, inet::IntrusivePtr<inet::Ipv4Header> dbfIpv4Header) {
    packet->popAtFront<inet::Ipv4Header>();
    packet->trimFront();
    packet->insertAtFront(dbfIpv4Header);
}

bool hasInfiniteDmax(DBFHeaderTag* dbfHeaderTag) {
    return dbfHeaderTag->getDMax() == SIMTIME_ZERO;
}

bool isExpired(DBFHeaderTag *dbfHeaderTag) {
    return !hasInfiniteDmax(dbfHeaderTag) && dbfHeaderTag->getTMax() < inet::simTime();
}

} // namespace
