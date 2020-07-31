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
#include <mutex>
#include "inet/queueing/scheduler/PriorityScheduler.h"
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeaderTag_m.h"

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
    bool isExpired(DBFHeaderTag *dbfHeaderTag);
    //void lookForMoreExpiredPackets();

  private:

    /**
     * The self message which
     * will be used for scheduling
     *
     */
    cMessage *selfMsg;

    /**
     * Counter for enqueued messages
     */
    int enqueuedMsgs;

    /**
     * The current scheduled packet
     */
    inet::Packet *currentScheduledPacket;

    /**
     * The index of the collection where
     * the current scheduled packet is queued
     */
    int currentCollectionsIdx;
};

} //namespace

#endif
