/*
 * DbfFibEntry.h
 *
 *  Created on: Aug 28, 2020
 *      Author: Mehmet Cakir
 */

#ifndef DELAYBASEDFORWARDING_NETWORKLAYER_COMPUTER_DBFFIBENTRY_H_
#define DELAYBASEDFORWARDING_NETWORKLAYER_COMPUTER_DBFFIBENTRY_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace delaybasedforwarding {

/**
 * @brief The delay based forwarding Forwarding Information Base (FIB) entry
 *
 * @ingroup delaybasedforwarding/networklayer/computer/
 *
 * @author Mehmet Cakir
 */
class DbfFibEntry {
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
