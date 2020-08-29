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

class DbfFibEntry {
public:
    DbfFibEntry(int hops, simtime_t lDelay, simtime_t hDelay, simtime_t fromDelay, simtime_t toDelay);
    virtual ~DbfFibEntry();
    const int getHops() const;
    const simtime_t& getLDelay() const;
    const simtime_t& getHDelay() const;
    const simtime_t& getFromDelay() const;
    const simtime_t& getToDelay() const;

private:
    const int hops;
    const simtime_t lDelay;
    const simtime_t hDelay;
    const simtime_t fromDelay;
    const simtime_t toDelay;
};

} /* namespace delaybasedforwarding */

#endif /* DELAYBASEDFORWARDING_NETWORKLAYER_COMPUTER_DBFFIBENTRY_H_ */
