/*
 * RandomDropHeaderSerializer.h
 *
 *  Created on: Feb 21, 2020
 *      Author: mehkir
 */

#ifndef INET_LINKLAYER_RANDOMDROPHEADER_RANDOMDROPHEADERSERIALIZER_H_
#define INET_LINKLAYER_RANDOMDROPHEADER_RANDOMDROPHEADERSERIALIZER_H_

#include "inet/common/packet/serializer/FieldsChunkSerializer.h"

using namespace inet;
namespace delaybasedforwarding {

class RandomDropHeaderSerializer : public FieldsChunkSerializer {
public:
    RandomDropHeaderSerializer();
    virtual ~RandomDropHeaderSerializer();

protected:
    virtual void serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const override;
    virtual const Ptr<Chunk> deserialize(MemoryInputStream& stream) const override;
};
}
#endif /* INET_LINKLAYER_RANDOMDROPHEADER_RANDOMDROPHEADERSERIALIZER_H_ */
