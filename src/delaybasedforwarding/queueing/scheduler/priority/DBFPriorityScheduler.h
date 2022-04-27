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

#ifndef __DELAYBASEDFORWARDING_QUEUEING_SCHEDULER_PRIORITY_DBFPRIORITYSCHEDULER_H_
#define __DELAYBASEDFORWARDING_QUEUEING_SCHEDULER_PRIORITY_DBFPRIORITYSCHEDULER_H_

//DBF
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeaderTag_m.h"
#include "delaybasedforwarding/queueing/classifier/DBFPriorityClassifier.h"
#include "delaybasedforwarding/queueing/scheduler/priority/DBFPriorityScheduleMsg_m.h"
//INET
#include <inet/queueing/scheduler/PriorityScheduler.h>
#include <inet/linklayer/ethernet/EtherMacFullDuplex.h>
//OMNET
#include <omnetpp.h>

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

  /**
   * Methods
   */
  public:
    /**
     * @brief Destructor
     */
    virtual ~DBFPriorityScheduler();

    /**
     * @brief Called whenever a subscribed signal is emitted and
     * is used to detect if Network Interface Card (NIC) is idle.
     *
     * @param source The object which emitted a signal
     * @param signalID
     * @param l The signal identifier of the emitted signal as a long value
     * @param details
     */
    virtual void receiveSignal(cComponent *source, simsignal_t signalID, long l, cObject *details) override;

    /**
     * @brief Called whenever a subscribed signal is emitted and
     * is used to record the number of queues currently in use.
     *
     * @param source The object which emitted a signal
     * @param signalID
     * @param value The of the emitted signal as a long value
     * @param details
     */
    virtual void receiveSignal(cComponent *source, simsignal_t signalID, cObject *value, cObject *details) override;

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
     * @param msg A scheduled self message or received message from another module
     */
    virtual void handleMessage(cMessage *msg) override;

    /**
     * @brief Is called whenever a new packet will be pushed in a queue
     *
     * @param gate The output gate of the queue where a packet is pushed in
     */
    virtual void handleCanPopPacket(cGate *gate) override;

    /**
     * @brief Schedules a packet to be sent
     */
    virtual void schedule();

    /**
     * @brief Looks for expired packets in all queues and deletes them.
     */
    virtual void lookForExpiredPackets();

    /**
     * @brief Determines if the given packet is ready to send.
     * Best-Effort(BE) packets are always ready.
     * DBF packets have must be checked according to their tMin time.
     * @param packet
     * @return true if packet is ready to send else false
     */
    bool isPacketReadyToSend(inet::Packet* packet);

  private:
    /**
     * @brief Determines the highest priority collection in which a packet is ready to be sent.
     *
     * @return index of the collection in which a packet is ready to be sent
     */
    int determineCollection();

  /**
   * Member variables
   */
  public:
  protected:
    /**
     * @brief Indicates the idle state of the mac module
     */
    int txIdleCounter;

    /**
     * @brief The DBF priority classifier
     */
    DBFPriorityClassifier *dbfPriorityClassifier;

    /**
     * @brief The self message which will be used for scheduling
     */
    DBFPriorityScheduleMsg *selfMsg;

  private:
    /**
     * @brief The MAC module which notifies about the availability of the Ethernet interface
     */
    inet::EtherMacFullDuplex *etherMacFullDuplex;

    /**
     * @brief The signal that emits the count of queues in use
     */
    static simsignal_t queuesInUseCountSignal;
};

} //namespace

#endif
