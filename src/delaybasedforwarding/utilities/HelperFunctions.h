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
 * @brief Determines if a packet contains the requested protocol
 *
 * @param packet the packet that is checked
 * @param requestedProtocol the protocol the packet is checked for
 * @return true if packet contains requested protocol
 *
 * @author Mehmet Cakir
 */
bool containsProtocol(inet::Packet *packet, const inet::Protocol *requestedProtocol);

/**
 * @brief Determines if a message contains an IPv4 header
 *
 * @param msg the message that is checked
 * @return true if message contains an IPv4 header
 *
 * @author Mehmet Cakir and Philipp Meyer
 */
bool containsIpv4Header(omnetpp::cMessage *msg);

/**
 * @brief Returns a duplicate of the IPv4 header in a packet
 *
 * @param packet including the IPv4 header
 * @return mutable duplicate of the IPv4 header
 *
 * @author Mehmet Cakir
 */
inet::IntrusivePtr<inet::Ipv4Header> getMutableIpv4Header(inet::Packet *packet);

/**
 * @brief Returns a duplicate of the delay-based forwarding IPv4 options in a IPv4 header
 *
 * @param dbfIpv4header where the IPv4 options are included
 * @return mutable duplicate of the IPv4 options
 *
 * @author Mehmet Cakir
 */
DBFIpv4Option* getMutableDBFIpv4Option(inet::IntrusivePtr<inet::Ipv4Header> dbfIpv4Header);

/**
 * @brief Removes delay-based forwarding options from an IPv4 header
 *
 * @param dbfIpv4Header The IPv4 header including the delay-based forwarding options
 *
 * @author Mehmet Cakir
 */
void removeDBFIpv4Options(inet::IntrusivePtr<inet::Ipv4Header> dbfIpv4Header);

/**
 * @brief Updates or replaces the IPv4 header in a packet with a given IPv4 header
 *
 * @param packet The packet including a IPv4 header
 * @param dbfIpv4Header The new IPv4 header which replaces the IPv4 header in a packet
 *
 * @author Mehmet Cakir
 */
void updateDBFIpv4Header(inet::Packet *packet, inet::IntrusivePtr<inet::Ipv4Header> dbfIpv4Header);

/**
 * @brief Determines if a delay-based forwarding (dbf) tag has an infinite upper bound Service Level Objective (SLO)
 *
 * @param dbfHeaderTag The dbf header tag that is checked
 * @return true if the dbf header tag contains an infinite upper bound SLO
 *
 * @author Mehmet Cakir
 */
bool hasInfiniteDmax(DBFHeaderTag* dbfHeaderTag);

/**
 * @brief Checks if the send time in the given delay-based forwarding (dbf) tag is exceeded
 *
 * @param dbfHeaderTag The given dbf tag
 *
 * @return true if send time is exceeded
 *
 * @author Mehmet Cakir
 */
bool isExpired(DBFHeaderTag *dbfHeaderTag);

} // namespace
#endif /* DELAYBASEDFORWARDING_UTILITIES_HELPERFUNCTIONS_H_ */
