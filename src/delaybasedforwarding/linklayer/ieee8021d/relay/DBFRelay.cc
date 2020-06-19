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

#include "DBFRelay.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "../../contract/randomDropHeader/RandomDropHeader_m.h"


namespace delaybasedforwarding {

Define_Module(DBFRelay);

void DBFRelay::handleLowerPacket(Packet *packet) {
    int arrivalInterfaceId = packet->getTag<InterfaceInd>()->getInterfaceId();
    InterfaceEntry *arrivalInterface = ifTable->getInterfaceById(arrivalInterfaceId);

    //check if arrival Interface isBoundaryPort --> dont check for header --> do nothing
    //else { check if header is present --> else throw error
    if (!arrivalInterface->par("isBoundaryPort").boolValue()) {
        //remove eth header
        const auto& eth = packet->popAtFront<EthernetMacHeader>();
        packet->popAtBack<EthernetFcs>(ETHER_FCS_BYTES);

        const auto& dropheader = packet->popAtFront<RandomDropHeader>();
        if (!dropheader) {
            throw cRuntimeError("ArrivalInterface ID = %d is not boundary. RandomDropHeader expected.", arrivalInterfaceId);
        }
        //if header contains 0 drop and return. if 1 --> remove it do nothing.
        if(dropheader->getDrop() == 0){
            delete packet;
            return;
        }
        //restore eth header
        packet->trim();
        packet->insertAtFront(eth);
        packet->insertAtBack(makeShared<EthernetFcs>(FCS_DECLARED_CORRECT));
    }
    Ieee8021dRelay::handleLowerPacket(packet);
}

void DBFRelay::dispatch(Packet *packet, InterfaceEntry *ie) {
    //check if target port is boundary port
    //if not add header with random 0/1
    if (!ie->par("isBoundaryPort").boolValue()){
        //remove eth header
        const auto& eth = packet->popAtFront<EthernetMacHeader>();
        packet->popAtBack<EthernetFcs>(ETHER_FCS_BYTES);
        packet->trim();

        int randomDropIndicator = rand() % 2;
        const auto& randomDropHeader = makeShared<RandomDropHeader>();
        randomDropHeader->setDrop(randomDropIndicator);
        packet->insertAtFront(randomDropHeader);
        packet->insertAtFront(eth);
        packet->insertAtBack(makeShared<EthernetFcs>(FCS_DECLARED_CORRECT));
    }
    Ieee8021dRelay::dispatch(packet, ie);
}

} //namespace
