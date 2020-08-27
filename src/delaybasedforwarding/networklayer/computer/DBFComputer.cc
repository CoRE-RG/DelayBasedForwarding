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

#include <delaybasedforwarding/networklayer/ipv4/DBFIpv4HeaderOptions_m.h>
#include "DBFComputer.h"
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeaderTag_m.h"
#include "delaybasedforwarding/utilities/HelperFunctions.h"
#include "inet/networklayer/ipv4/Ipv4Header_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"

namespace delaybasedforwarding {

#define STARTEDELAY 0.0

// TODO do we need to include node ?
#define THISNODE 0

#define ETHERNET_HEADER_BITS 26.0*8.0 // includes ETHER_MAC_FRAME_BYTES(18B) + ETHER_PHY_HEADER(8B)
#define ETHERNET_MIN_PAYLOAD_BITS 46.0*8.0

Define_Module(DBFComputer);

void DBFComputer::initialize()
{
    dMin = par("dMin");
    dMax = par("dMax");
    admit = par("admit");
    cModule *network = getModuleByPath("<root>");
    cableDelay = SimTime(network->par("_delay"));
    cableLength = network->par("_length");
    cableDatarate = network->par("_datarate");

    dbfFIB = new std::map<inet::Ipv4Address, int>();

    cXMLElement *config = par("dbfFIB").xmlValue();
    cXMLElementList nodes = config->getChildrenByTagName("node");
    for (int i = 0; i < nodes.size(); i++) {
        cXMLElement* node = nodes[i];
        const char* ip = node->getAttribute("ip");
        const inet::L3Address address = inet::L3AddressResolver().resolve(ip);
        const int hops = atoi(node->getAttribute("hops"));
        EV_DEBUG << ip << " has hops: " << hops << endl;
        dbfFIB->insert({address.toIpv4(), hops});
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
    calculate(packet);
}

void DBFComputer::calculate(inet::Packet *packet) {
    auto ipv4Header = packet->peekAtFront<inet::Ipv4Header>();
    auto dbfIpv4Header = inet::IntrusivePtr<inet::Ipv4Header>(ipv4Header->dup());
    const inet::TlvOptionBase *tlvOptionBase = dbfIpv4Header->findOptionByType(DBFIpv4OptionType::DBFPARAMETERS);
    DBFIpv4Option *dbfIpv4Option = dynamic_cast<DBFIpv4Option*>(tlvOptionBase->dup());

    auto dbfHeaderTag = packet->findTag<DBFHeaderTag>();
    dbfHeaderTag->setDMin(dbfIpv4Option->getDMin());
    dbfHeaderTag->setDMax(dbfIpv4Option->getDMax());
    dbfHeaderTag->setEDelay(dbfIpv4Option->getEDelay());
    dbfHeaderTag->setAdmit(dbfIpv4Option->getAdmit());
    dbfHeaderTag->setToHops(dbfFIB->at(dbfIpv4Header->getDestAddress()));


    // Calculate link dependent delays
    double ethPadding = (double)packet->getBitLength() >= ETHERNET_MIN_PAYLOAD_BITS ? 0.0 : ETHERNET_MIN_PAYLOAD_BITS - (double)packet->getBitLength();
    simtime_t ldelay = SimTime(cableDelay.dbl() + (double)(packet->getBitLength() + ETHERNET_HEADER_BITS + ethPadding) / cableDatarate);
    simtime_t hdelay = ldelay;
    simtime_t fromdelay = SimTime((double)dbfFIB->at(dbfIpv4Header->getSrcAddress()) * ldelay.dbl());
    simtime_t todelay = SimTime((double)dbfHeaderTag->getToHops() * ldelay.dbl()); //TODO add with calculated toDelay from fib, don't forget to take out cableDelay from calculation before
    dbfHeaderTag->setLDelay(ldelay);
    dbfHeaderTag->setHDelay(hdelay);
    dbfHeaderTag->setFromDelay(fromdelay);
    dbfHeaderTag->setToDelay(todelay);

    // Update experienced delay (eDelay)
    if (dbfHeaderTag->getFromNetwork()) {
        dbfHeaderTag->setEDelay(dbfHeaderTag->getEDelay() + dbfHeaderTag->getLDelay());
    }
    inet::TlvOptions &tlvOptions = dbfIpv4Header->getOptionsForUpdate();
    tlvOptions.deleteOptionByType(DBFIpv4OptionType::DBFPARAMETERS, false);
    dbfIpv4Option->setEDelay(dbfHeaderTag->getEDelay());
    dbfIpv4Header->addOption(dbfIpv4Option);

    packet->popAtFront<inet::Ipv4Header>();
    packet->trimFront();
    packet->insertAtFront(dbfIpv4Header);

    // Calculate queueing budget and send time
    simtime_t fqdelayMin = dbfHeaderTag->getDMin() - dbfHeaderTag->getToDelay() - dbfHeaderTag->getEDelay();
    simtime_t fqdelayMax = dbfHeaderTag->getDMax() - dbfHeaderTag->getToDelay() - dbfHeaderTag->getEDelay();
    dbfHeaderTag->setLqBudgetMin(SimTime(fqdelayMin.dbl() / (double)(dbfHeaderTag->getToHops() + THISNODE)));
    dbfHeaderTag->setLqBudgetMax(SimTime(fqdelayMax.dbl() / (double)(dbfHeaderTag->getToHops()+ THISNODE)));
    dbfHeaderTag->setTMin(dbfHeaderTag->getLqBudgetMin() + dbfHeaderTag->getTRcv());
    dbfHeaderTag->setTMax(dbfHeaderTag->getLqBudgetMax() + dbfHeaderTag->getTRcv());
}

bool DBFComputer::isAlreadyExpired(inet::Packet *packet) {
    auto dbfHeaderTag = packet->findTag<DBFHeaderTag>();
    return dbfHeaderTag->getTMax() < dbfHeaderTag->getTRcv();
}

} //namespace
