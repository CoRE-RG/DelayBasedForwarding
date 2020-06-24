/*
 * RandomDropHeaderSerializer.cpp
 *
 *  Created on: Feb 21, 2020
 *      Author: mehkir
 */

#include "RandomDropHeaderSerializer.h"
#include "RandomDropHeader_m.h"
#include "inet/common/packet/serializer/ChunkSerializerRegistry.h"

using namespace inet;

namespace delaybasedforwarding {

Register_Serializer(RandomDropHeader, RandomDropHeaderSerializer);

RandomDropHeaderSerializer::RandomDropHeaderSerializer() : FieldsChunkSerializer() {
}

RandomDropHeaderSerializer::~RandomDropHeaderSerializer() {
}

void RandomDropHeaderSerializer::serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const {
    const auto& header = staticPtrCast<const RandomDropHeader>(chunk);
    stream.writeUint32Be(header->getDrop());
}

const Ptr<Chunk> RandomDropHeaderSerializer::deserialize(MemoryInputStream& stream) const {
    auto header = makeShared<RandomDropHeader>();
    header->setDrop(stream.readUint32Be());
    return header;
}
}
