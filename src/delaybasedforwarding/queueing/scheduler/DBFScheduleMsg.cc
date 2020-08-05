/*
 * DBFScheduleMsg.cc
 *
 *  Created on: Aug 5, 2020
 *      Author: mehkir
 */

#include <delaybasedforwarding/queueing/scheduler/DBFScheduleMsg.h>

namespace delaybasedforwarding {

Register_Class(DBFScheduleMsg)

void DBFScheduleMsg::copy(const DBFScheduleMsg& other) {
    this->scheduledPacket = other.scheduledPacket;
}

inet::Packet* DBFScheduleMsg::getScheduledPacket() {
    return scheduledPacket;
}

void DBFScheduleMsg::setScheduledPacket(inet::Packet *packet) {
    scheduledPacket = packet;
}

} /* namespace delaybasedforwarding */
