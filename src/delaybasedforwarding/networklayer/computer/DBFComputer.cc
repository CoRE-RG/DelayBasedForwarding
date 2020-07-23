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

namespace delaybasedforwarding {

#define STARTEDELAY 0


Define_Module(DBFComputer);

void DBFComputer::initialize()
{
    dMin = par("dMin");
    dMax = par("dMax");
    admit = par("admit");
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

} //namespace
