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

#include "inet/common/ProtocolTools.h"
#include "DBFIpv4.h"
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeader_m.h"
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeaderTag_m.h"
#include "inet/common/packet/serializer/ChunkSerializerRegistry.h"

namespace delaybasedforwarding {

Define_Module(DBFIpv4);

void DBFIpv4::initialize(int stage) {
    inet::Ipv4::initialize(stage);
    if (stage == inet::INITSTAGE_LOCAL) {
        if (!(dbfComputer = dynamic_cast<DBFComputer*>(getParentModule()->getParentModule()->getSubmodule("dbfComputer")))) {
            throw cRuntimeError("Could not get DBFComputer in module %s", this->getFullName());
        }
    }
}

void DBFIpv4::sendPacketToNIC(inet::Packet *packet) {
    dbfComputer->processDBFPacket(packet);
    if (packet) {
        Ipv4::sendPacketToNIC(packet);
    }
}

void DBFIpv4::encapsulate(inet::Packet *packet) {
    auto dbfHeader = inet::makeShared<DBFHeader>();
    dbfComputer->addSLOPrameters(dbfHeader);
    auto dbfHeaderTag = packet->addTag<DBFHeaderTag>();
    dbfHeaderTag->setTRcv(simTime());
    dbfHeaderTag->setFromNetwork(false);

    if (findPacketProtocol(packet) == &inet::Protocol::udp) {
        inet::Ipv4::encapsulate(packet);
        auto ipv4Header = packet->popAtFront<inet::Ipv4Header>();
        packet->trimFront();
        packet->insertAtFront(dbfHeader);
        packet->insertAtFront(ipv4Header);
    } else {
        packet->insertAtFront(dbfHeader);
        inet::Ipv4::encapsulate(packet);
    }
}

void DBFIpv4::decapsulate(inet::Packet *packet) {
    inet::Ipv4::decapsulate(packet);
    packet->popAtFront<DBFHeader>();
}

} //namespace
