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

#include "IngressForwarder.h"
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeaderTag_m.h"
#include "delaybasedforwarding/utilities/HelperFunctions.h"
//TODO remove timetag include and creation time tag dependent uses
#include "inet/common/TimeTag_m.h"

namespace delaybasedforwarding {

Define_Module(IngressForwarder);

void IngressForwarder::initialize()
{
}

void IngressForwarder::handleMessage(cMessage *msg)
{
    if (containsIpv4Header(msg)) {
        attachTrcv(msg);
    }
    send(msg,"out");
}

void IngressForwarder::attachTrcv(cMessage *msg) {
    inet::Packet *packet = dynamic_cast<inet::Packet*>(msg);


    //########################Remove from here ... ###################################
    auto data = packet->peekData();
    auto regions = data->getAllTags<inet::CreationTimeTag>();
    const SimTime simtime = inet::SimTime::parse("246.857090645169s");
    for (auto &region : regions) {
        if (region.getTag()->getCreationTime() == simtime) {
            EV_DEBUG << "Problematic packet arrived in " << this->getFullName() << endl;
        }
    }
    //########################Remove till here ... ###################################

    auto dbfHeaderTag = packet->addTag<DBFHeaderTag>();
    dbfHeaderTag->setTRcv(simTime());
    dbfHeaderTag->setFromNetwork(true);
}

} //namespace
