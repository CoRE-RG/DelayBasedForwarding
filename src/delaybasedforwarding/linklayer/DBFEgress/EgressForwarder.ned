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
        //Signal emitted for frame transmitted to particular port, contains sending packet
        @signal[txPkToPort_*](type=simtime_t);
        //Signal emitted when a frame is transmitted, contains sending packet
        @signal[txPks](type=inet::Packet);
        //End-to-end latency for packets per port
        @statisticTemplate[txPk](title="End-to-end latency"; unit=s; record=stats,histogram?,vector; interpolationmode=none);
        //End-to-end latency for packets
        @statistic[txPks](title="End-to-end latency"; source="messageAge(txPks)"; unit=s; record=stats,histogram?,vector; interpolationmode=none);
    
    gates:
        input in;
        output out;
        input forward @labels(pop);
        output schedule @lables(push);
}
