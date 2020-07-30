/*
 * DBFIpv4HeaderSerializer.h
 *
 *  Created on: Jul 30, 2020
 *      Author: Mehmet Cakir
 */

#ifndef DELAYBASEDFORWARDING_NETWORKLAYER_IPV4_DBFIPV4HEADERSERIALIZER_H_
#define DELAYBASEDFORWARDING_NETWORKLAYER_IPV4_DBFIPV4HEADERSERIALIZER_H_

#include "inet/networklayer/ipv4/Ipv4HeaderSerializer.h"

namespace delaybasedforwarding {

class DBFIpv4HeaderSerializer: public inet::Ipv4HeaderSerializer {

protected:
  virtual void serializeOption(inet::MemoryOutputStream& stream, const inet::TlvOptionBase *option) const override;
  virtual inet::TlvOptionBase *deserializeOption(inet::MemoryInputStream& stream) const override;

  virtual void serialize(inet::MemoryOutputStream& stream, const inet::Ptr<const inet::Chunk>& chunk) const override;
  virtual const inet::Ptr<inet::Chunk> deserialize(inet::MemoryInputStream& stream) const override;

public:
    DBFIpv4HeaderSerializer(): inet::Ipv4HeaderSerializer() {}
};

} /* namespace delaybasedforwarding */

#endif /* DELAYBASEDFORWARDING_NETWORKLAYER_IPV4_DBFIPV4HEADERSERIALIZER_H_ */
