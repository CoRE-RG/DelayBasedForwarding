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

package delaybasedforwarding.linklayer.DBFEgress;

//
// @brief This module implements the forwarder at egress for delay based forwarded packets
//
// @ingroup delaybasedforwarding/linklayer/DBFEgress
//
// @author Mehmet Cakir
//
simple EgressForwarder
{
    parameters:
        //Signal emitted when a frame is received, contains received EtherFrame
        @signal[txPkToPort_*](type=inet::Packet);
        //End-to-end latency
        @statisticTemplate[txPk](title="End-to-end latency"; unit=s; record=stats,histogram?,vector; interpolationmode=none);
    
    gates:
        input in;
        output out;
        input forward @labels(pop);
        output schedule @lables(push);
}
