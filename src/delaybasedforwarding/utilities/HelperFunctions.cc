/*
 * HelperFunctions.cc
 *
 *  Created on: Jul 2, 2020
 *      Author: mehkir
 */

#include "HelperFunctions.h"
#include "inet/networklayer/ipv4/Ipv4Header_m.h"

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

} // namespace
