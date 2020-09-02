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
#include "delaybasedforwarding/queueing/scheduler/DBFScheduleMsg.h"
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
  public:
    virtual ~DBFPriorityScheduler();
    virtual void receiveSignal(cComponent *source, simsignal_t signalID, long l, cObject *details) override;
    int addDBFQueue(cModule *dbfQueue);

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void handleCanPopPacket(cGate *gate) override;
    virtual int schedulePacket() override;

  private:
    void checkQueues();
    bool isExpired(DBFHeaderTag *dbfHeaderTag);
    void lookForExpiredPackets();

  private:

    /**
     * The self message which
     * will be used for scheduling
     */
    DBFScheduleMsg *selfMsg;

    /**
     * Indicates the idle state
     * of the mac module
     */
    int txIdleCounter;

    /**
     * The MAC module which notifies about
     * the availability of the Ethernet interface
     */
    inet::EtherMacFullDuplex *etherMacFullDuplex;

    /**
     * The DBF priority classifier
     */
    DBFPriorityClassifier *dbfPriorityClassifier;
};

} //namespace

#endif
