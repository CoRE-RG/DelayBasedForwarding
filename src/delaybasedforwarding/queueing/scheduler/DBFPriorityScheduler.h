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

#ifndef __DELAYBASEDFORWARDING_DBFPRIORITYSCHEDULER_H_
#define __DELAYBASEDFORWARDING_DBFPRIORITYSCHEDULER_H_

#include <omnetpp.h>
#include <inet/queueing/scheduler/PriorityScheduler.h>

using namespace omnetpp;

namespace delaybasedforwarding {

/**
 * @brief The priority scheduler for delay based forwarded packets
 *
 * @ingroup delaybasedforwarding/queueing/scheduler
 *
 * @author Mehmet Cakir
 */

class DBFPriorityScheduler : public inet::queueing::PriorityScheduler
{
  public:
    virtual ~DBFPriorityScheduler();

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void handleCanPopPacket(cGate *gate) override;

  private:
    void checkQueues();

  private:
    enum ScheduleKind {DBFPacket,NONDBFPacket};

    /**
     * The self message which will be scheduled
     * for delay based forwarding packets
     */
    cMessage *dbfSelfMsg;

    /**
     * The self message which will be scheduled
     * for non delay based forwarding packets
     */
    cMessage *nonDBFSelfMsg;

    /**
     * Counter for enqueued messages
     */
    int enqueuedMsgs;

    /**
     * The collections index for a scheduled delay based forwarding packet
     */
    int currentDBFCollectionsIdx;

    /**
     * The collections index for a non scheduled delay based forwarding packet
     */
    int currentNONDBFCollectionsIdx;

    /**
     * The current schedule time
     */
    simtime_t currentScheduleTime;
};

} //namespace

#endif
