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
#include "inet/queueing/scheduler/PriorityScheduler.h"
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeaderTag_m.h"
#include "delaybasedforwarding/queueing/scheduler/DBFScheduleMsg_m.h"
#include "inet/linklayer/ethernet/EtherMacFullDuplex.h"
#include "delaybasedforwarding/queueing/classifier/DBFPriorityClassifier.h"

using namespace omnetpp;

namespace delaybasedforwarding {

/**
 * @brief The priority scheduler for delay based forwarded packets
 *
 * @ingroup delaybasedforwarding/queueing/scheduler
 *
 * @author Mehmet Cakir
 */

class DBFPriorityScheduler : public inet::queueing::PriorityScheduler, public cListener
{

  private:
    /**
     * @brief Looks for a new higher prioritized packet and schedules it
     */
    void checkQueues();

    /**
     * @brief Looks for the first queue containing packets beginning from the highest priority queue.
     * Expired packets in that queue will be removed and deleted.
     */
    void lookForExpiredPackets();

  public:
    /**
     * @brief Destructor
     */
    virtual ~DBFPriorityScheduler();

    /**
     * @brief Is called whenever a subscribed signal is emitted
     *
     * @param source The object which emitted a signal
     * @param signalID
     * @param l The signal identifier of the emitted signal as a long value
     * @param details
     */
    virtual void receiveSignal(cComponent *source, simsignal_t signalID, long l, cObject *details) override;

    /**
     * @brief Adds a delay-based forwarding queue to this scheduler and connects it with this scheduler
     *
     * @param dbfQueue The delay-based forwarding queue which will be added to this scheduler and connected with this scheduler
     *
     * @return The index of the queue in the providers list of this scheduler
     */
    int addDBFQueue(cModule *dbfQueue);

  protected:
    /**
     * @brief The initialize method
     *
     * @param stage The stage of the initialization process
     */
    virtual void initialize(int stage) override;

    /**
     * @brief The handle message method
     *
     * @param msg A scheduled self message or received message
     */
    virtual void handleMessage(cMessage *msg) override;

    /**
     * @brief Is called whenever a new packet will be pushed in a queue
     *
     * @param gate The output gate of the queue where a packet is pushed in
     */
    virtual void handleCanPopPacket(cGate *gate) override;

    /**
     * @brief Looks for the packet at the front of all queues and checks them for their minimal send time.
     *  The index of the collection where the packet with the lowest minimal send time is contained will be returned.
     *
     *  @return index of the collection where the packet with the lowest minimal send time is contained
     */
    virtual int schedulePacket() override;

  private:

    /**
     * @brief The self message which will be used for scheduling
     */
    DBFScheduleMsg *selfMsg;

    /**
     * @brief Indicates the idle state of the mac module
     */
    int txIdleCounter;

    /**
     * @brief The MAC module which notifies about the availability of the Ethernet interface
     */
    inet::EtherMacFullDuplex *etherMacFullDuplex;

    /**
     * @brief The DBF priority classifier
     */
    DBFPriorityClassifier *dbfPriorityClassifier;
};

} //namespace

#endif
