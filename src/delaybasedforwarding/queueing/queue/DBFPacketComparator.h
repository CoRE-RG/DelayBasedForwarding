/*
 * DBFPacketComparator.h
 *
 *  Created on: Jul 19, 2020
 *      Author: mehkir
 */

#ifndef DELAYBASEDFORWARDING_QUEUEING_QUEUE_DBFPACKETCOMPARATOR_H_
#define DELAYBASEDFORWARDING_QUEUEING_QUEUE_DBFPACKETCOMPARATOR_H_

#include <inet/queueing/contract/IPacketComparatorFunction.h>

namespace delaybasedforwarding {

class DBFPacketComparator: public omnetpp::cObject, public inet::queueing::IPacketComparatorFunction {
public:
    DBFPacketComparator();
    virtual ~DBFPacketComparator();

    /**
     * Returns the priority among the packets:
     * result < 0: packet1 has higher priority
     * result > 0: packet2 has higher priority
     */
     virtual int comparePackets(inet::Packet *packet1, inet::Packet *packet2) const override;
     virtual DBFPacketComparator *dup() const override;
};

} /* namespace delaybasedforwarding */

#endif /* DELAYBASEDFORWARDING_QUEUEING_QUEUE_DBFPACKETCOMPARATOR_H_ */
