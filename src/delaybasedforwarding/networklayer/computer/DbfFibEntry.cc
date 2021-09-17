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
