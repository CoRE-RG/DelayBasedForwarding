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

#include "DBFIpv4.h"
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeader_m.h"

namespace delaybasedforwarding {

Define_Module(DBFIpv4);

void DBFIpv4::encapsulate(inet::Packet *packet) {
    // Insert DBF Header at front
    auto dbfHeader = inet::makeShared<DBFHeader>();
    packet->insertAtFront(dbfHeader);
    inet::Ipv4::encapsulate(packet);
}

void DBFIpv4::decapsulate(inet::Packet *packet) {
    inet::Ipv4::decapsulate(packet);
    // Pop at front DBF Header
    packet->popAtFront<DBFHeader>();
}

} //namespace
