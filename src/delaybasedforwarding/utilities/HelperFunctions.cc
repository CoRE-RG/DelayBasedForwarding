/*
 * HelperFunctions.cc
 *
 *  Created on: Jul 2, 2020
 *      Author: mehkir
 */

#include "HelperFunctions.h"
#include "inet/networklayer/ipv4/Ipv4Header_m.h"
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeader_m.h"

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

bool containsDBFHeader(omnetpp::cMessage *msg) {
    bool containsDBFHeader = false;
    if (inet::Packet *packet = dynamic_cast<inet::Packet*>(msg)) {
        if (containsProtocol(packet, &inet::Protocol::ipv4)) {
            containsDBFHeader = true;
        }
    }
    return containsDBFHeader;
}

void updateEDelay(inet::Packet *packet, omnetpp::simtime_t eDelay) {
    auto oldDBFHeader = packet->popAtFront<DBFHeader>();
    auto updatedDBFHeader = inet::makeShared<DBFHeader>();
    updatedDBFHeader->setAdmit(oldDBFHeader->getAdmit());
    updatedDBFHeader->setDMax(oldDBFHeader->getDMax());
    updatedDBFHeader->setDMin(oldDBFHeader->getDMin());
    updatedDBFHeader->setEDelay(eDelay);
    packet->trimFront();
    packet->insertAtFront(updatedDBFHeader);
}

} // namespace
