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

#ifndef __DELAYBASEDFORWARDING_QUEUEING_SCHEDULER_DYNAMICPRIORITY_DBFDYNAMICPRIORITYSCHEDULER_H_
#define __DELAYBASEDFORWARDING_QUEUEING_SCHEDULER_DYNAMICPRIORITY_DBFDYNAMICPRIORITYSCHEDULER_H_

//DBF
#include "delaybasedforwarding/queueing/scheduler/priority/DBFPriorityScheduler.h"
#include "delaybasedforwarding/queueing/scheduler/dynamicpriority/DBFDynamicPriorityScheduleMsg_m.h"
//OMNET
#include <omnetpp.h>

using namespace omnetpp;

namespace delaybasedforwarding {

/**
 * @brief The dynamic priority scheduler for delay based forwarded packets
 *
 * @ingroup delaybasedforwarding/queueing/scheduler
 *
 * @author Mehmet Mueller
 */

class DBFDynamicPriorityScheduler : public DBFPriorityScheduler
{
  /**
   * Methods
   */
  public:
  protected:
    /**
     * @brief The handle message method
     *
     * @param msg A scheduled self message or received message from another module
     */
    virtual void handleMessage(cMessage *msg) override;

    /**
     * @brief Schedules a packet to be sent
     */
    virtual void schedule() override;

    /**
     * @brief Looks for expired packets in all queues and deletes them.
     */
    virtual void lookForExpiredPackets() override;
  private:
    /**
     * @brief Prepares the selfMsg with the highest priority collection in which a packet is ready to be sent
     * and with the highest priority packet itself.
     */
    void prepareSelfMsg();

    /**
     * @brief Calculates the dynamic priority of the given packet.
     * @param packet the given packet
     * @return the priority
     */
    double calculateDynamicPriority(inet::Packet* packet);

    /**
     * @brief Checks if the given packet is a DBF packet.
     * @param packet the given packet
     * @return true if contains DBFHeaderTag
     */
    bool isDBFPacket(inet::Packet* packet);

  /**
   * Member variables
   */
  public:
  protected:
  private:
    /**
     * @brief The self message which will be used for scheduling
     */
    DBFDynamicPriorityScheduleMsg *selfMsg;
};

} //namespace

#endif
