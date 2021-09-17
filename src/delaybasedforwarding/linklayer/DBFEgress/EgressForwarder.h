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

#ifndef __DELAYBASEDFORWARDING_FORWARDER_H_
#define __DELAYBASEDFORWARDING_FORWARDER_H_

#include <omnetpp.h>
#include <map>
#include "inet/common/IProtocolRegistrationListener.h"

using namespace omnetpp;

namespace delaybasedforwarding {

/**
 * @brief The forwarder at egress for delay-based forwarded packets
 *
 * @ingroup delaybasedforwarding/linklayer/DBFEgress
 *
 * @author Mehmet Cakir
 */
class EgressForwarder : public cSimpleModule, public inet::IProtocolRegistrationListener
{
  private:
    /**
     * @brief Emits every specific flow by its destination port
     *
     * @param msg The message which is emitted
     */
    void emitPacket(cMessage *msg);

  protected:
    /**
     * @brief The initialize method
     *
     * @param stage The stage of the initialization process
     */
    virtual void initialize() override;

    /**
     * @brief The handle message method
     *
     * @param msg A scheduled self message or received message
     */
    virtual void handleMessage(cMessage *msg) override;

    /**
     * @brief Passes the registration of the service through
     *
     * @param protocol
     * @param out
     * @param servicePrimitive
     */
    virtual void handleRegisterService(const inet::Protocol& protocol, cGate *out, inet::ServicePrimitive servicePrimitive) override;

    /**
     * @brief Passes the registration of the protocol through
     *
     * @param protocol
     * @param in
     * @param servicePrimitive
     */
    virtual void handleRegisterProtocol(const inet::Protocol& protocol, cGate *in, inet::ServicePrimitive servicePrimitive) override;

  private:

    /**
     * @brief The map containing all signals for specific flows to emit them by their destination port
     */
    std::map<uint32_t,simsignal_t> txPktToPortSignals;

    /**
     * @brief Signal that emits a packet when it is sent
     */
    static simsignal_t txPksSignal;

};

} //namespace

#endif
