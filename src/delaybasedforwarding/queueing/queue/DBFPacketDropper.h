/*
 * DBFPacketDropper.h
 *
 *  Created on: Dec 4, 2020
 *      Author: mehkir
 */

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
     */
    virtual inet::Packet *selectPacket(inet::queueing::IPacketCollection *collection) const override;
};

} /* namespace delaybasedforwarding */

#endif /* DELAYBASEDFORWARDING_QUEUEING_QUEUE_DBFPACKETDROPPER_H_ */
