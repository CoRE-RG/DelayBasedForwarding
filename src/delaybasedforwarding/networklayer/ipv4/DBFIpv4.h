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

#ifndef __DELAYBASEDFORWARDING_DBFIPV4_H_
#define __DELAYBASEDFORWARDING_DBFIPV4_H_

//DBF
#include "delaybasedforwarding/networklayer/computer/DBFComputer.h"
//OMNET
#include <omnetpp.h>
//INET
#include <inet/networklayer/ipv4/Ipv4.h>


using namespace omnetpp;

namespace delaybasedforwarding {

/**
 * @brief The Ipv4 module delay base forwarded packets
 *
 * @ingroup delaybasedforwarding/networklayer/ipv4
 *
 * @author Mehmet Cakir
 */
class DBFIpv4 : public inet::Ipv4
{
  protected:
    /**
     * @brief The initialize method
     *
     * @param stage The stage of the initialization process
     */
    virtual void initialize(int stage) override;

    /**
     * @brief Encapsulates packets coming from higher layers into Ipv4Header
     *
     * @param packet The packet coming from application layer
     */
    virtual void encapsulate(inet::Packet *packet) override;

    /**
     * Sends packet to the network interface controller
     *
     * @param packet The packet processed by the network layer
     */
    virtual void sendPacketToNIC(inet::Packet *packet) override;

  private:
    /**
     * @brief The reference of the DBFComputer module
     */
    DBFComputer *dbfComputer = nullptr;
};

} //namespace

#endif
