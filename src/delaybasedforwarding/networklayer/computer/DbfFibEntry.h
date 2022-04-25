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

#ifndef DELAYBASEDFORWARDING_NETWORKLAYER_COMPUTER_DBFFIBENTRY_H_
#define DELAYBASEDFORWARDING_NETWORKLAYER_COMPUTER_DBFFIBENTRY_H_

//OMNET
#include <omnetpp.h>

using namespace omnetpp;

namespace delaybasedforwarding {

/**
 * @brief The delay based forwarding Forwarding Information Base(FIB) entry
 *
 * @ingroup delaybasedforwarding/networklayer/computer/
 *
 * @author Mehmet Cakir
 */
class DbfFibEntry {
  /**
   * Methods
   */
  public:
    /**
     * @brief Constructor
     *
     * @param hops The hops between the actual node to another node
     * @param lDelay The delay from the predecessor node to the actual node
     * @param hDelay The delay from the actual node to its successor node
     * @param fromDelay The delay from the origin node to the actual node
     * @param toDelay The delay from the actual node to the destination node
     */
    DbfFibEntry(int hops, simtime_t lDelay, simtime_t hDelay, simtime_t fromDelay, simtime_t toDelay);

    /**
     * @brief Destructor
     */
    virtual ~DbfFibEntry();

    /**
     * @brief Returns the hops between the actual node to another node
     *
     * @return The hop count between the actual node to another node
     */
    const int getHops() const;

    /**
     * @brief Returns the delay from the predecessor node to the actual node
     *
     * @return delay from the predecessor node to the actual node
     */
    const simtime_t& getLDelay() const;

    /**
     * @brief Returns the delay from the actual node to its successor node
     *
     * @return delay from the actual node to its successor node
     */
    const simtime_t& getHDelay() const;

    /**
     * @brief Returns the delay from the origin node to the actual node
     *
     * @return delay from the origin node to the actual node
     */
    const simtime_t& getFromDelay() const;

    /**
     * @brief Returns the delay from the actual node to the destination node
     *
     * @return delay from the actual node to the destination node
     */
    const simtime_t& getToDelay() const;
  protected:
  private:

  /**
   * Member variables
   */
  public:
  protected:
  private:
    /**
     * @brief The hops between the actual node to another node
     */
    const int hops;

    /**
     * @brief The delay from the predecessor node to the actual node
     */
    const simtime_t lDelay;

    /**
     * @brief The delay from the actual node to its successor node
     */
    const simtime_t hDelay;

    /**
     * @brief The delay from the origin node to the actual node
     */
    const simtime_t fromDelay;

    /**
     * @brief The delay from the actual node to the destination node
     */
    const simtime_t toDelay;
};

} /* namespace delaybasedforwarding */

#endif /* DELAYBASEDFORWARDING_NETWORKLAYER_COMPUTER_DBFFIBENTRY_H_ */
