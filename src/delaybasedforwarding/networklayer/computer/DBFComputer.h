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

#ifndef __DELAYBASEDFORWARDING_DBFCOMPUTER_H_
#define __DELAYBASEDFORWARDING_DBFCOMPUTER_H_

#include <omnetpp.h>
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeader_m.h"
#include "inet/common/packet/Packet.h"

using namespace omnetpp;

namespace delaybasedforwarding {

/**
 * @brief The parameter computer for delay based forwarded packets
 *
 * @ingroup delaybasedforwarding/networklayer/computer/
 *
 * @author Mehmet Cakir
 */
class DBFComputer : public cSimpleModule
{

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

  public:
    void addSLOParameters(inet::Ptr<DBFHeader> dbfHeader);
    void processDBFPacket(inet::Packet *packet);

  private:
    simtime_t dMin;
    simtime_t dMax;
    uint32_t admit;
    int fromHops;
    int toHops;
    double cableLength;
    double cableDatarate;
    simtime_t cableDelay;

  private:
    void calculate(inet::Packet *packet);
    bool isAlreadyExpired(inet::Packet *packet);
};

} //namespace

#endif
