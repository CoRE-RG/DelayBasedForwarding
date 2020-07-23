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
#include "inet/networklayer/ipv4/Ipv4Header_m.h"
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeaderTag_m.h"
#include "delaybasedforwarding/utilities/HelperFunctions.h"

namespace delaybasedforwarding {

// TODO do we need to include node ?
#define THISNODE 0

Define_Module(IngressForwarder);

void IngressForwarder::initialize()
{
    cModule *network = getModuleByPath("<root>");
    fromHops = par("fromHops");
    toHops = par("toHops");
    cableDelay = SimTime(network->par("_delay"));
    cableLength = network->par("_length");
    cableDatarate = network->par("_datarate");
}

void IngressForwarder::handleMessage(cMessage *msg)
{
    if (toHops && containsDBFHeader(msg)) {
        processDBFPacket(msg);
        if (!isAlreadyExpired(msg)) {
            send(msg,"out");
        } else {
            delete msg;
        }
    } else {
        send(msg,"out");
    }
}

bool IngressForwarder::containsDBFHeader(cMessage *msg) {
    bool containsDBFHeader = false;
    if (inet::Packet *packet = dynamic_cast<inet::Packet*>(msg)) {
        if (containsProtocol(packet, &inet::Protocol::ipv4)) {
            containsDBFHeader = true;
        }
    }
    return containsDBFHeader;
}

void IngressForwarder::processDBFPacket(cMessage *msg) {
    inet::Packet *packet = dynamic_cast<inet::Packet*>(msg);
    auto ipv4Header = packet->popAtFront<inet::Ipv4Header>();
    auto dbfHeader = packet->peekAtFront<DBFHeader>();
    calculate(packet, dbfHeader);
    packet->trimFront();
    packet->insertAtFront(ipv4Header);
}

void IngressForwarder::calculate(inet::Packet *packet, inet::Ptr<const DBFHeader> dbfHeader) {
    auto dbfHeaderTag = packet->addTag<DBFHeaderTag>();
    dbfHeaderTag->setTRcv(simTime());
    dbfHeaderTag->setDMin(dbfHeader->getDMin());
    dbfHeaderTag->setDMax(dbfHeader->getDMax());
    dbfHeaderTag->setEDelay(dbfHeader->getEDelay());
    dbfHeaderTag->setAdmit(dbfHeader->getAdmit());
    dbfHeaderTag->setToHops(toHops);

    // Calculate link dependent delays
    simtime_t ldelay = SimTime(cableDelay.dbl() + (double)packet->getBitLength()/cableDatarate);
    simtime_t hdelay = ldelay;
    simtime_t fromdelay = SimTime((double)fromHops * ldelay.dbl());
    simtime_t todelay = SimTime((double)toHops * ldelay.dbl());
    dbfHeaderTag->setLDelay(ldelay);
    dbfHeaderTag->setHDelay(hdelay);
    dbfHeaderTag->setFromDelay(fromdelay);
    dbfHeaderTag->setToDelay(todelay);

    // Update experienced delay (eDelay)
    dbfHeaderTag->setEDelay(dbfHeaderTag->getEDelay() + dbfHeaderTag->getLDelay());

    // Calculate queueing budget and send time
    simtime_t fqdelayMin = dbfHeaderTag->getDMin() - dbfHeaderTag->getToDelay() - dbfHeaderTag->getEDelay();
    simtime_t fqdelayMax = dbfHeaderTag->getDMax() - dbfHeaderTag->getToDelay() - dbfHeaderTag->getEDelay();
    dbfHeaderTag->setLqBudgetMin(SimTime(fqdelayMin.dbl() / (double)(toHops + THISNODE)));
    dbfHeaderTag->setLqBudgetMax(SimTime(fqdelayMax.dbl() / (double)(toHops + THISNODE)));
    dbfHeaderTag->setTMin(dbfHeaderTag->getLqBudgetMin() + dbfHeaderTag->getTRcv());
    dbfHeaderTag->setTMax(dbfHeaderTag->getLqBudgetMax() + dbfHeaderTag->getTRcv());
}

bool IngressForwarder::isAlreadyExpired(cMessage *msg) {
    auto packet = dynamic_cast<inet::Packet*>(msg);
    auto dbfHeaderTag = packet->findTag<DBFHeaderTag>();
    return dbfHeaderTag->getTMax() < dbfHeaderTag->getTRcv();
}

} //namespace
