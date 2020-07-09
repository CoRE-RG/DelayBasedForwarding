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

#ifndef __DELAYBASEDFORWARDING_SCHEDULER_H_
#define __DELAYBASEDFORWARDING_SCHEDULER_H_

#include <omnetpp.h>
#include <inet/queueing/contract/IActivePacketSource.h>
#include <inet/queueing/contract/IActivePacketSink.h>

using namespace omnetpp;

namespace delaybasedforwarding {

/**
 * @brief The scheduler for delay based forwarded packets
 *
 * @ingroup delaybasedforwarding/linklayer/DBFEgress
 *
 * @author Mehmet Cakir
 */
class Scheduler : public cSimpleModule, public inet::queueing::IActivePacketSource, public inet::queueing::IActivePacketSink
{
  protected:
    cGate *enqueueGate = nullptr;
    inet::queueing::IPassivePacketSink *consumer = nullptr;

  public:
    cGate *dequeueGate = nullptr;
    inet::queueing::IPassivePacketSource *provider = nullptr;

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;

    /**
     * Returns the passive packet sink where packets are pushed. The gate must
     * not be nullptr.
     */
    virtual inet::queueing::IPassivePacketSink *getConsumer(cGate *gate) override;

    /**
     * Notifies about a state change that allows to push some packet into the
     * passive packet sink at the given gate. The gate is never nullptr.
     */
    virtual void handleCanPushPacket(cGate *gate) override;

  public:
    /**
     * Returns the passive packet source from where packets are collected. The
     * gate must not be nullptr.
     */
    virtual inet::queueing::IPassivePacketSource *getProvider(cGate *gate) override;

    /**
     * Notifies about a state change that allows to pop some packet from the
     * passive packet source at the given gate. The gate is never nullptr.
     */
    virtual void handleCanPopPacket(cGate *gate) override;

};

} //namespace

#endif
