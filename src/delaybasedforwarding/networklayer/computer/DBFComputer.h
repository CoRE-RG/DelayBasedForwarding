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

//DBF
#include "delaybasedforwarding/networklayer/computer/DbfFibEntry.h"
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeaderTag_m.h"
#include "delaybasedforwarding/networklayer/ipv4/DBFIpv4HeaderOptions_m.h"
//INET
#include <inet/networklayer/ipv4/Ipv4Header_m.h>
#include <inet/common/packet/Packet.h>

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

  /**
   * Methods
   */
  public:
    /**
    * @brief Adds the initial Service Level Objective (SLO) parameters
    *
    * @param packet The packet which is populated with the SLO parameters
    */
    void addSLOParameters(inet::Packet *packet);

    /**
    * @brief Processes the delay-based forwarding packet
    *
    * @param packet The packet which is processed
    */
    void processDBFPacket(inet::Packet *packet);

    /**
    * @brief Checks if the packet is expired
    *
    * @param packet The packet which is checked
    *
    * @return true if the maximum send time is already exceeded
    */
    bool isAlreadyExpired(inet::Packet *packet);

    /**
    * @brief Checks if packets should be sent as best-effort packtets
    *
    * @return true if lower and upper bound of Service Level Objective are set to zero
    */
    bool isBEMode();

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
     * @brief Prepares a delay-based forwarding (dbf) header tag for further calculation
     *
     * @param packet The packet whose tag parameters are set
     * @param dbfIpv4Option The dbf options which are used for the dbf header tag
     * @param dbfIpv4Header The dbf IPv4 header of the packet
     *
     * @return the modified dbf header tag
     */
    DBFHeaderTag* prepareDBFTag(inet::Packet *packet, DBFIpv4Option *dbfIpv4Option, inet::IntrusivePtr<inet::Ipv4Header> dbfIpv4Header);

    /**
     * @brief Calculates the transmission time of the next hop
     *
     * @param packet The packet the transmission time is calculated for
     *
     * @return The transmission time
     */
    simtime_t calculateTransmissionTime(inet::Packet *packet);

    /**
     * @brief Updates the experienced delay by adding the experienced delay
     * between the predecessor node and the actual node
     *
     * @param dbfHeaderTag The dbf header tag which is updated
     * @param transmissionTime The calculated transmissionTime which is used for calculating the experienced delay
     */
    void updateEdelay(DBFHeaderTag *dbfHeaderTag, simtime_t transmissionTime);

    /**
     * @brief Calculates the delay between the actual node to another including the transmission time
     *
     * @param hops The hops between the actual and another node
     * @param transmissionTime The transmission time over the link
     * @param delay The delay from the Forwarding Information Base
     *
     * @return delay between the actual node to another
     */
    simtime_t getDelay(double hops, double transmissionTime, double delay);

    /**
     * @brief Calculates the queuing budget in total left
     *
     * @param dBound The lower/upper bound of the Service Level Objective
     * @param dbfHeaderTag The delay-based forwarding header tag used for the calculation
     *
     * @return the minimum/maximum queuing budget in total left
     */
    simtime_t calculateFqDelay(simtime_t dBound, DBFHeaderTag* dbfHeaderTag);

    /**
     * @brief Calculates the local queuing budget
     *
     * @param fqdelay The minimum/maximum queuing budget in total left
     * @param dbfHeaderTag The delay-based forwarding header tag used for the calculation
     *
     * @return the minimum/maximum local queuing budget
     */
    simtime_t calculateLqBudget(simtime_t fqDelay, DBFHeaderTag* dbfHeaderTag);

    /**
     * @brief Calculates the minimum/maximum send time
     *
     * @param lqbudget The minumum/maximum local queuing budget
     * @param dbfHeaderTag The delay-based forwarding header tag used for the calculation
     *
     * @return the minimum/maximum send time
     */
    simtime_t calculateSendTime(simtime_t lqBudget, DBFHeaderTag* dbfHeaderTag);

  public:
  protected:
  private:
    /**
     * @brief The lower bound of the Service Level Objective (SLO)
     */
    simtime_t dMin;

    /**
     * @brief The upper bound of the Service Level Objective (SLO)
     */
    simtime_t dMax;

    /**
     * @brief The admit bit of the Service Level Objective (SLO)
     */
    uint32_t admit;

    /**
     * @brief The cable data rate of the network
     */
    double cableDatarate;

    /**
     * @brief The delay-based forwarding Forwarding Information Base(FIB) map
     */
    std::map<inet::Ipv4Address, DbfFibEntry> *dbfFIB;

};

} //namespace

#endif
