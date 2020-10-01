/*
 * HelperFunctions.h
 *
 *  Created on: Jul 2, 2020
 *      Author: mehkir
 */

#ifndef DELAYBASEDFORWARDING_UTILITIES_HELPERFUNCTIONS_H_
#define DELAYBASEDFORWARDING_UTILITIES_HELPERFUNCTIONS_H_

#include "inet/common/ProtocolTag_m.h"
#include "inet/common/packet/Packet_m.h"
#include "inet/networklayer/ipv4/Ipv4Header_m.h"
#include "delaybasedforwarding/networklayer/ipv4/DBFIpv4HeaderOptions_m.h"
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeaderTag_m.h"

namespace delaybasedforwarding {

/**
 * Returns true if packet contains requested protocol
 */
bool containsProtocol(inet::Packet *packet, const inet::Protocol *requestedProtocol);
bool containsIpv4Header(omnetpp::cMessage *msg);
inet::IntrusivePtr<inet::Ipv4Header> getMutableIpv4Header(inet::Packet *packet);
DBFIpv4Option* getMutableDBFIpv4Option(inet::IntrusivePtr<inet::Ipv4Header> dbfIpv4Header);
void removeDBFIpv4Options(inet::IntrusivePtr<inet::Ipv4Header> dbfIpv4Header);
void updateDBFIpv4Header(inet::Packet *packet, inet::IntrusivePtr<inet::Ipv4Header> dbfIpv4Header);
bool hasInfiniteDmax(DBFHeaderTag* dbfHeaderTag);

} // namespace
#endif /* DELAYBASEDFORWARDING_UTILITIES_HELPERFUNCTIONS_H_ */
