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

package delaybasedforwarding.networklayer.computer;

//
// The parameter computer for delay based forwarded packets
//
// @author Mehmet Cakir
//
simple DBFComputer
{
    parameters:
        @class(DBFComputer);
        double dMin @unit(s) = default(0.0s);  // The lower bound of the Service Level Object (SLO)
        double dMax @unit(s) = default(0.0s);  // The upper bound of the SLO
        xml dbfFIB;                            // The delay-based forwarding Forwarding Information Base
        int admit = default(0);                // The admit bit to distinguish dbf packets with the same SLO but different admit bit
}
