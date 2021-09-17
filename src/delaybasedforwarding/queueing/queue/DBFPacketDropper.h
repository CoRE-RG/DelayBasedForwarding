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

#ifndef DELAYBASEDFORWARDING_QUEUEING_QUEUE_DBFPACKETDROPPER_H_
#define DELAYBASEDFORWARDING_QUEUEING_QUEUE_DBFPACKETDROPPER_H_

#include <inet/queueing/contract/IPacketDropperFunction.h>

namespace delaybasedforwarding {

/**
 * @brief The delay-based forwarding packet dropper
 *
 * @ingroup delaybasedforwarding/queueing/queue
 *
 * @author Mehmet Cakir
 */

class DBFPacketDropper: public omnetpp::cObject, public inet::queueing::IPacketDropperFunction {
public:

    /**
     * @brief Constructor
     */
    DBFPacketDropper();

    /**
     * @brief Destructor
     */
    virtual ~DBFPacketDropper();

    /**
     * @brief Returns a packet to be dropped from the collection.
     *
     * @param collection the collection from which a packet will be dropped
     *
     * @return the packet to be dropped
     */
    virtual inet::Packet *selectPacket(inet::queueing::IPacketCollection *collection) const override;
};

} /* namespace delaybasedforwarding */

#endif /* DELAYBASEDFORWARDING_QUEUEING_QUEUE_DBFPACKETDROPPER_H_ */
