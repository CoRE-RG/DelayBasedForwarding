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
#include "inet/common/IProtocolRegistrationListener.h"

using namespace omnetpp;

namespace delaybasedforwarding {

/**
 * @brief The forwarder at egress for delay based forwarded packets
 *
 * @ingroup delaybasedforwarding/linklayer/DBFEgress
 *
 * @author Mehmet Cakir
 */
class EgressForwarder : public cSimpleModule, public inet::IProtocolRegistrationListener
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void handleRegisterService(const inet::Protocol& protocol, cGate *out, inet::ServicePrimitive servicePrimitive) override;
    virtual void handleRegisterProtocol(const inet::Protocol& protocol, cGate *in, inet::ServicePrimitive servicePrimitive) override;

  private:
    bool containsDBFHeader(cMessage *msg);
    void processDBFPacket(cMessage *msg);
};

} //namespace

#endif
