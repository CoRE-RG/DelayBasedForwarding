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

#ifndef DELAYBASEDFORWARDING_QUEUEING_QUEUE_DBFPACKETCOMPARATOR_H_
#define DELAYBASEDFORWARDING_QUEUEING_QUEUE_DBFPACKETCOMPARATOR_H_

#include <inet/queueing/contract/IPacketComparatorFunction.h>

namespace delaybasedforwarding {

/**
 * @brief The delay-based forwarding packet comparator
 *
 * @ingroup delaybasedforwarding/queueing/queue
 *
 * @author Mehmet Cakir
 */
class DBFPacketComparator: public omnetpp::cObject, public inet::queueing::IPacketComparatorFunction {
public:
    /**
     * @brief Constructor
     */
    DBFPacketComparator();

    /**
     * @brief Destructor
     */
    virtual ~DBFPacketComparator();

    /**
     * @brief Returns the priority among two packets
     *
     * @param packet1 The packet which will be pushed in the queue
     * @param packet2 A packet which is already in the queue
     *
     * @return result < 0: packet1 has higher priority, result >= 0: packet2 has higher priority
     */
     virtual int comparePackets(inet::Packet *packet1, inet::Packet *packet2) const override;

     /**
      * @brief Returns a new DBFPacketComparator
      *
      * @return A new DBFPacketComparator
      */
     virtual DBFPacketComparator *dup() const override;
};

} /* namespace delaybasedforwarding */

#endif /* DELAYBASEDFORWARDING_QUEUEING_QUEUE_DBFPACKETCOMPARATOR_H_ */
