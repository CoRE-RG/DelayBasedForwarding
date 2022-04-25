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

//OMNET
#include <omnetpp.h>

using namespace omnetpp;

namespace delaybasedforwarding {

/**
 * @brief The forwarder at ingress for delay-based forwarded packets
 *
 * @ingroup delaybasedforwarding/linklayer/DBFIngress
 *
 * @author Mehmet Cakir
 */
class IngressForwarder : public cSimpleModule
{
  private:
    /**
     * @brief Attaches the time of receipt to a message
     *
     * @param msg The message the time of receipt is attached
     */
    void attachTrcv(cMessage *msg);

    /**
     * @brief Checks if a message contains delay-based forwarding options
     *
     * @param msg The message that is checked
     *
     * @return true if the message contains delay-based forwarding options
     */
    bool containsDBFOptions(cMessage *msg);

  protected:
    /**
     * @brief The initialize method
     *
     * @param stage The stage of the initialization process
     */
    virtual void initialize();

    /**
     * @brief The handle message method
     *
     * @param msg A scheduled self message or received message
     */
    virtual void handleMessage(cMessage *msg);

  private:
    /**
     * @brief The signal that is emitted when a packet is received
     */
    static simsignal_t rxPkSignal;
};

} //namespace

#endif
