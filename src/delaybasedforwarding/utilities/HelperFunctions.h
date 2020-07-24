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

namespace delaybasedforwarding {

/**
 * Returns true if packet contains requested protocol
 */
bool containsProtocol(inet::Packet *packet, const inet::Protocol *requestedProtocol);
bool containsDBFHeader(omnetpp::cMessage *msg);
void updateEDelay(inet::Packet *packet, omnetpp::simtime_t eDelay);

} // namespace
#endif /* DELAYBASEDFORWARDING_UTILITIES_HELPERFUNCTIONS_H_ */
