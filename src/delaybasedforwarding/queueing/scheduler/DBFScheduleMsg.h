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

class DBFScheduleMsg : public DBFScheduleMsg_Base {

    /**
     * Methods
     */
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
     */
    DBFScheduleMsg(const DBFScheduleMsg& other) : DBFScheduleMsg_Base(other)
    {
        copy(other);
    }

    /**
     * @brief Assignment operator
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
     */
    virtual DBFScheduleMsg *dup() const
    {
        return new DBFScheduleMsg(*this);
    }

    inet::Packet* getScheduledPacket();

    void setScheduledPacket(inet::Packet *packet);

private:
    void copy(const DBFScheduleMsg& other);

    /**
     * Member variables
     */
private:
    inet::Packet *scheduledPacket;
};

} /* namespace delaybasedforwarding */

#endif /* DELAYBASEDFORWARDING_QUEUEING_SCHEDULER_DBFSCHEDULEMSG_H_ */
