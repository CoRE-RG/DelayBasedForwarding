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

enum DBFIpv4OptionType {
    DBFPARAMETERS = 2020;
}

class DBFIpv4Option extends inet::TlvOptionBase
{
    type = DBFPARAMETERS;
    length = 28;
    simtime_t dMin = 0;
    simtime_t dMax = 0;
    simtime_t eDelay = 0;
    uint32_t admit = 0;
}