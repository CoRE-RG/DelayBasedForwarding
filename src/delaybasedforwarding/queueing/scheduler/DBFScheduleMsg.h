/*
 * DBFScheduleMsg.h
 *
 *  Created on: Aug 5, 2020
 *      Author: mehkir
 */

#ifndef DELAYBASEDFORWARDING_QUEUEING_SCHEDULER_DBFSCHEDULEMSG_H_
#define DELAYBASEDFORWARDING_QUEUEING_SCHEDULER_DBFSCHEDULEMSG_H_

#include "delaybasedforwarding/queueing/scheduler/DBFScheduleMsg_m.h"
#include "inet/common/packet/Packet.h"

namespace delaybasedforwarding {

/**
 * @brief The delay-based forwarding schedule message to schedule a delay-based forwarding (dbf) packet
 *
 * @ingroup delaybasedforwarding/queueing/scheduler
 *
 * @author Mehmet Cakir
 */
class DBFScheduleMsg : public DBFScheduleMsg_Base {

/**
 * Methods
 */
private:
    /**
     * @brief Copies the scheduled dbf packet within a dbf schedule message to this dbf schedule message
     *
     * @param other The dbf schedule message from which the scheduled packet to be copied
     */
    void copy(const DBFScheduleMsg& other);

public:
    /**
     * @brief Constructor
     *
     * @param newName message name
     * @param kind message type
     */
    DBFScheduleMsg(const char *newName=nullptr, short kind=0) : DBFScheduleMsg_Base(newName,kind) {
    }

    /**
     * @brief Copy constructor
     *
     * @param other An other dbf schedule message
     */
    DBFScheduleMsg(const DBFScheduleMsg& other) : DBFScheduleMsg_Base(other)
    {
        copy(other);
    }

    /**
     * @brief Assignment operator
     *
     * @param other The dbf schedule message which has to be assigned to this message
     *
     * @return The assigned dbf schedule message
     */
    DBFScheduleMsg& operator=(const DBFScheduleMsg& other)
    {
        if (this==&other) return *this;
        DBFScheduleMsg_Base::operator=(other);
        copy(other);
        return *this;
    }

    /**
     * @brief DBFScheduleMsg duplication
     *
     * @return The duplicate of this dbf schedule message
     */
    virtual DBFScheduleMsg *dup() const
    {
        return new DBFScheduleMsg(*this);
    }

    /**
     * @brief Returns the scheduled dbf packet
     *
     * @return the scheduled dbf packet
     */
    inet::Packet* getScheduledPacket();

    /**
     * @brief Sets the dbf packet which has to be scheduled
     *
     * @param packet The dbf packet which has to be scheduled
     */
    void setScheduledPacket(inet::Packet *packet);


    /**
     * Member variables
     */
    private:
        /**
         * @brief The scheduled dbf packet
         */
        inet::Packet *scheduledPacket;

};
} /* namespace delaybasedforwarding */

#endif /* DELAYBASEDFORWARDING_QUEUEING_SCHEDULER_DBFSCHEDULEMSG_H_ */
