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

package delaybasedforwarding.linklayer.DBFIngress;

//
// The forwarder at ingress for delay-based forwarded packets
//
// @author Mehmet Cakir
//
simple IngressForwarder
{
    parameters:
        //Signal emitted when a frame is received, contains received EtherFrame
        @signal[rxPk](type=inet::Packet);
        //End-to-end latency statistic
        @statistic[rxLatency](title="End-to-end latency"; source="messageAge(rxPk)"; unit=s; record=stats,histogram?,vector; interpolationmode=none);
           
    gates:
        // the input gate
        input in;
        // the output gate
        output out;
}
