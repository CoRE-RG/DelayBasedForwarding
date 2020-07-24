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
#include "delaybasedforwarding/linklayer/contract/dbf/DBFHeaderTag_m.h"
#include "delaybasedforwarding/utilities/HelperFunctions.h"
#include "inet/networklayer/ipv4/Ipv4Header_m.h"

namespace delaybasedforwarding {

#define STARTEDELAY 0

// TODO do we need to include node ?
#define THISNODE 0

Define_Module(DBFComputer);

void DBFComputer::initialize()
{
    dMin = par("dMin");
    dMax = par("dMax");
    admit = par("admit");
    cModule *network = getModuleByPath("<root>");
    fromHops = par("fromHops");
    toHops = par("toHops");
    cableDelay = SimTime(network->par("_delay"));
    cableLength = network->par("_length");
    cableDatarate = network->par("_datarate");
}

void DBFComputer::handleMessage(cMessage *msg)
{
    // Do nothing
}

void DBFComputer::addSLOPrameters(inet::Ptr<DBFHeader> dbfHeader)
{
    dbfHeader->setDMin(dMin);
    dbfHeader->setDMax(dMax);
    dbfHeader->setAdmit(admit);
    dbfHeader->setEDelay(STARTEDELAY);
}

void DBFComputer::processDBFPacket(inet::Packet *packet) {
    auto ipv4Header = packet->popAtFront<inet::Ipv4Header>();
    calculate(packet);
    packet->trimFront();
    packet->insertAtFront(ipv4Header);
    if (isAlreadyExpired(packet)) {
        delete packet;
    }
}

void DBFComputer::calculate(inet::Packet *packet) {
    auto dbfHeader = packet->peekAtFront<DBFHeader>();
    auto dbfHeaderTag = packet->findTag<DBFHeaderTag>();
    dbfHeaderTag->setDMin(dbfHeader->getDMin());
    dbfHeaderTag->setDMax(dbfHeader->getDMax());
    dbfHeaderTag->setEDelay(dbfHeader->getEDelay());
    dbfHeaderTag->setAdmit(dbfHeader->getAdmit());
    dbfHeaderTag->setToHops(toHops);

    // Calculate link dependent delays
    simtime_t ldelay = SimTime(cableDelay.dbl() + (double)packet->getBitLength() / cableDatarate);
    simtime_t hdelay = ldelay;
    simtime_t fromdelay = SimTime((double)fromHops * ldelay.dbl());
    simtime_t todelay = SimTime((double)toHops * ldelay.dbl());
    dbfHeaderTag->setLDelay(ldelay);
    dbfHeaderTag->setHDelay(hdelay);
    dbfHeaderTag->setFromDelay(fromdelay);
    dbfHeaderTag->setToDelay(todelay);

    // Update experienced delay (eDelay)
    if (dbfHeaderTag->getFromNetwork()) {
        dbfHeaderTag->setEDelay(dbfHeaderTag->getEDelay() + dbfHeaderTag->getLDelay());
        updateEDelay(packet, dbfHeaderTag->getEDelay());
    }

    // Calculate queueing budget and send time
    simtime_t fqdelayMin = dbfHeaderTag->getDMin() - dbfHeaderTag->getToDelay() - dbfHeaderTag->getEDelay();
    simtime_t fqdelayMax = dbfHeaderTag->getDMax() - dbfHeaderTag->getToDelay() - dbfHeaderTag->getEDelay();
    dbfHeaderTag->setLqBudgetMin(SimTime(fqdelayMin.dbl() / (double)(toHops + THISNODE)));
    dbfHeaderTag->setLqBudgetMax(SimTime(fqdelayMax.dbl() / (double)(toHops + THISNODE)));
    dbfHeaderTag->setTMin(dbfHeaderTag->getLqBudgetMin() + dbfHeaderTag->getTRcv());
    dbfHeaderTag->setTMax(dbfHeaderTag->getLqBudgetMax() + dbfHeaderTag->getTRcv());
}

bool DBFComputer::isAlreadyExpired(inet::Packet *packet) {
    auto dbfHeaderTag = packet->findTag<DBFHeaderTag>();
    return dbfHeaderTag->getTMax() < dbfHeaderTag->getTRcv();
}

} //namespace
