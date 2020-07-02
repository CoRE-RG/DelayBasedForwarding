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

} // namespace
