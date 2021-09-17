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

#include "DBFComputer.h"
#include "delaybasedforwarding/utilities/HelperFunctions.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include <exception>

namespace delaybasedforwarding {

#define STARTEDELAY 0.0

// TODO do we need to include current node ?
#define THISNODE 0

#define ETHERNET_HEADER_BITS 26.0*8.0 // includes ETHER_MAC_FRAME_BYTES(18B) + ETHER_PHY_HEADER(8B)
#define ETHERNET_MIN_PAYLOAD_BITS 46.0*8.0

Define_Module(DBFComputer);

void DBFComputer::initialize()
{
    dMin = par("dMin");
    dMax = par("dMax");
    if (dMin > 0.0 && dMax > 0.0 && dMin > dMax) {
        throw cRuntimeError("dMax must be greater than dMin.");
    }
    if (dMin < 0.0 || dMax < 0.0) {
        throw cRuntimeError("dMin and dMax must be positive");
    }
    admit = par("admit");
    cModule *network = getModuleByPath("<root>");
    cableDatarate = 0.0;
    if (network->hasPar("_datarate")) {
        cableDatarate = network->par("_datarate");
    }


    dbfFIB = new std::map<inet::Ipv4Address, DbfFibEntry>();

    cXMLElement *config = par("dbfFIB").xmlValue();
    cXMLElementList nodes = config->getChildrenByTagName("node");
    for (size_t i = 0; i < nodes.size(); i++) {
        cXMLElement* node = nodes[i];
        const char* ip = node->getAttribute("ip");
        const inet::L3Address address = inet::L3AddressResolver().resolve(ip);
        const int hops = atoi(node->getAttribute("hops"));
        const simtime_t lDelay = SimTime::parse(node->getAttribute("lDelay"));
        const simtime_t hDelay = SimTime::parse(node->getAttribute("hDelay"));
        const simtime_t fromDelay = SimTime::parse(node->getAttribute("fromDelay"));
        const simtime_t toDelay = SimTime::parse(node->getAttribute("toDelay"));
        dbfFIB->insert({address.toIpv4(), DbfFibEntry(hops,lDelay,hDelay,fromDelay,toDelay)});
    }
}

void DBFComputer::handleMessage(cMessage *msg)
{
    // Do nothing
}

void DBFComputer::addSLOParameters(inet::Packet *packet)
{
    DBFIpv4Option *dbfIpv4Option = new DBFIpv4Option();
    dbfIpv4Option->setDMin(dMin);
    dbfIpv4Option->setDMax(dMax);
    dbfIpv4Option->setAdmit(admit);
    dbfIpv4Option->setEDelay(SimTime(STARTEDELAY));

    auto ipv4Header = packet->popAtFront<inet::Ipv4Header>();
    packet->trimFront();
    auto dbfIpv4Header = inet::IntrusivePtr<inet::Ipv4Header>(ipv4Header->dup());
    dbfIpv4Header->addOption(dbfIpv4Option);
    packet->insertAtFront(dbfIpv4Header);
}

void DBFComputer::processDBFPacket(inet::Packet *packet) {
    inet::IntrusivePtr<inet::Ipv4Header> dbfIpv4Header = getMutableIpv4Header(packet);
    DBFIpv4Option *dbfIpv4Option = getMutableDBFIpv4Option(dbfIpv4Header);
    DBFHeaderTag* dbfHeaderTag = prepareDBFTag(packet, dbfIpv4Option, dbfIpv4Header);

    simtime_t transmissionTime = calculateTransmissionTime(packet);
    simtime_t fromdelay = getDelay((double)dbfHeaderTag->getFromHops(), transmissionTime.dbl(), dbfFIB->at(dbfIpv4Header->getSrcAddress()).getFromDelay().dbl());
    simtime_t todelay = getDelay((double)dbfHeaderTag->getToHops(), transmissionTime.dbl(), dbfFIB->at(dbfIpv4Header->getDestAddress()).getToDelay().dbl());
    dbfHeaderTag->setFromDelay(fromdelay);
    dbfHeaderTag->setToDelay(todelay);

    updateEdelay(dbfHeaderTag, transmissionTime);
    removeDBFIpv4Options(dbfIpv4Header);
    dbfIpv4Option->setEDelay(dbfHeaderTag->getEDelay());
    dbfIpv4Header->addOption(dbfIpv4Option);
    updateDBFIpv4Header(packet, dbfIpv4Header);

    // Calculate queuing budget and send time
    simtime_t fqdelayMin = SIMTIME_ZERO;
    if (dbfHeaderTag->getDMin() != SIMTIME_ZERO) {
        fqdelayMin = calculateFqDelay(dbfHeaderTag->getDMin(), dbfHeaderTag);
    }
    dbfHeaderTag->setLqBudgetMin(calculateLqBudget(fqdelayMin, dbfHeaderTag));
    dbfHeaderTag->setTMin(calculateSendTime(dbfHeaderTag->getLqBudgetMin(), dbfHeaderTag));

    simtime_t fqdelayMax = SIMTIME_ZERO;
    if (!hasInfiniteDmax(dbfHeaderTag)){
        fqdelayMax = calculateFqDelay(dbfHeaderTag->getDMax(), dbfHeaderTag);
    }
    dbfHeaderTag->setLqBudgetMax(calculateLqBudget(fqdelayMax, dbfHeaderTag));
    dbfHeaderTag->setTMax(calculateSendTime(dbfHeaderTag->getLqBudgetMax(), dbfHeaderTag));
}

bool DBFComputer::isAlreadyExpired(inet::Packet *packet) {
    auto dbfHeaderTag = packet->findTag<DBFHeaderTag>();
    return !hasInfiniteDmax(dbfHeaderTag) && dbfHeaderTag->getTMax() < dbfHeaderTag->getTRcv();
}

bool DBFComputer::isBEMode() {
    return dMin == SIMTIME_ZERO && dMax == SIMTIME_ZERO;
}

DBFHeaderTag* DBFComputer::prepareDBFTag(inet::Packet *packet, DBFIpv4Option *dbfIpv4Option, inet::IntrusivePtr<inet::Ipv4Header> dbfIpv4Header) {
    DBFHeaderTag* dbfHeaderTag = packet->findTag<DBFHeaderTag>();
    dbfHeaderTag->setDMin(dbfIpv4Option->getDMin());
    dbfHeaderTag->setDMax(dbfIpv4Option->getDMax());
    dbfHeaderTag->setEDelay(dbfIpv4Option->getEDelay());
    dbfHeaderTag->setAdmit(dbfIpv4Option->getAdmit());
    dbfHeaderTag->setFromHops(dbfFIB->at(dbfIpv4Header->getSrcAddress()).getHops());
    dbfHeaderTag->setToHops(dbfFIB->at(dbfIpv4Header->getDestAddress()).getHops());
    dbfHeaderTag->setLDelay(dbfFIB->at(dbfIpv4Header->getSrcAddress()).getLDelay());
    dbfHeaderTag->setHDelay(dbfFIB->at(dbfIpv4Header->getDestAddress()).getHDelay());
    return dbfHeaderTag;
}

simtime_t DBFComputer::calculateTransmissionTime(inet::Packet *packet) {
    simtime_t transmissionTime = SIMTIME_ZERO;
    if (cableDatarate != 0.0) {
        double ethPadding = (double)packet->getBitLength() >= ETHERNET_MIN_PAYLOAD_BITS ? 0.0 : ETHERNET_MIN_PAYLOAD_BITS - (double)packet->getBitLength();
        transmissionTime = SimTime((double)(packet->getBitLength() + ETHERNET_HEADER_BITS + ethPadding) / cableDatarate);
    }
    return transmissionTime;
}

void DBFComputer::updateEdelay(DBFHeaderTag *dbfHeaderTag, simtime_t transmissionTime) {
    if (dbfHeaderTag->getFromNetwork()) {
        dbfHeaderTag->setEDelay(dbfHeaderTag->getEDelay() + dbfHeaderTag->getLDelay() + transmissionTime);
    }
}

simtime_t DBFComputer::getDelay(double hops, double transmissionTime, double delay) {
    return SimTime(hops * transmissionTime + delay);
}

simtime_t DBFComputer::calculateFqDelay(simtime_t dBound, DBFHeaderTag* dbfHeaderTag) {
    return dBound - dbfHeaderTag->getToDelay() - dbfHeaderTag->getEDelay();
}

simtime_t DBFComputer::calculateLqBudget(simtime_t fqDelay, DBFHeaderTag* dbfHeaderTag) {
    return SimTime(fqDelay.dbl() / (double)dbfHeaderTag->getToHops());
}

simtime_t DBFComputer::calculateSendTime(simtime_t lqBudget, DBFHeaderTag* dbfHeaderTag) {
    return lqBudget + dbfHeaderTag->getTRcv();
}

} //namespace
