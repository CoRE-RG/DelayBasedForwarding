/*
 * DBFPacketComparator.h
 *
 *  Created on: Jul 19, 2020
 *      Author: Mehmet Cakir
 */

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
