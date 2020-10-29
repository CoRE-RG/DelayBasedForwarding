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

import inet.common.TlvOptions;

namespace delaybasedforwarding;

//
// The enum for delay-based forwarding options
//
// @author Mehmet Cakir
// 
enum DBFIpv4OptionType {
    DBFPARAMETERS = 2020; // The delay-based forwarding parameters type 
}

//
// This class implements the delay-based forwarding options parameters for the Ipv4 header
//
// @author Mehmet Cakir
// 
class DBFIpv4Option extends inet::TlvOptionBase
{
    type = DBFPARAMETERS; // The type of the delay-based forwarding options
    length = 28;          // The length of a DBFIpv4Option instance
    simtime_t dMin = 0;   // The lower bound of the Service Level Objective (SLO)
    simtime_t dMax = 0;   // The upper bound of the SLO
    simtime_t eDelay = 0; // The experienced delay of the SLO
    uint32_t admit = 0;   // The admit bit of the SLO
}