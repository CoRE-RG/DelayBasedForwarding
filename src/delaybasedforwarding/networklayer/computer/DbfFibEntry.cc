/*
 * DbfFibEntry.cc
 *
 *  Created on: Aug 28, 2020
 *      Author: mehkir
 */

#include <delaybasedforwarding/networklayer/computer/DbfFibEntry.h>

namespace delaybasedforwarding {

DbfFibEntry::DbfFibEntry(int hops, simtime_t lDelay, simtime_t hDelay, simtime_t fromDelay, simtime_t toDelay) :
        hops(hops), lDelay(lDelay), hDelay(hDelay), fromDelay(fromDelay), toDelay(toDelay) {
}

const int DbfFibEntry::getHops() const {
    return hops;
}

const simtime_t& DbfFibEntry::getLDelay() const {
    return lDelay;
}

const simtime_t& DbfFibEntry::getHDelay() const {
    return hDelay;
}

const simtime_t& DbfFibEntry::getFromDelay() const {
    return fromDelay;
}

const simtime_t& DbfFibEntry::getToDelay() const {
    return toDelay;
}

DbfFibEntry::~DbfFibEntry() {
}

} /* namespace delaybasedforwarding */
