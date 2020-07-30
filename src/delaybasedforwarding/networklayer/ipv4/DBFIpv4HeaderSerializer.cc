/*
 * DBFIpv4HeaderSerializer.cc
 *
 *  Created on: Jul 30, 2020
 *      Author: mehkir
 */

#include <delaybasedforwarding/networklayer/ipv4/DBFIpv4HeaderSerializer.h>
#include "inet/common/packet/serializer/ChunkSerializerRegistry.h"

namespace inet {
Register_Serializer(Ipv4Header, delaybasedforwarding::DBFIpv4HeaderSerializer);
}

namespace delaybasedforwarding {

void DBFIpv4HeaderSerializer::serializeOption(inet::MemoryOutputStream& stream, const inet::TlvOptionBase *option) const {
    inet::Ipv4HeaderSerializer::serializeOption(stream, option);
}

inet::TlvOptionBase* DBFIpv4HeaderSerializer::deserializeOption(inet::MemoryInputStream& stream) const {
    return inet::Ipv4HeaderSerializer::deserializeOption(stream);
}

void DBFIpv4HeaderSerializer::serialize(inet::MemoryOutputStream& stream, const inet::Ptr<const inet::Chunk>& chunk) const {
    inet::Ipv4HeaderSerializer::serialize(stream, chunk);
}

const inet::Ptr<inet::Chunk> DBFIpv4HeaderSerializer::deserialize(inet::MemoryInputStream& stream) const {
    return inet::Ipv4HeaderSerializer::deserialize(stream);
}

} /* namespace delaybasedforwarding */
