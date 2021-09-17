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

package delaybasedforwarding.applications;

import inet.applications.udpapp.UdpBasicBurst;

//
// Configurable UDP burst application 
//
// @author Mehmet Cakir
//
simple DBFUdpBurst extends UdpBasicBurst {
    parameters:
        // The interval the burst is periodically started
        double burstInterval @unit(s);       
        // The burst rate per second in bit 
        double burstRatePerSecond @unit(bps);
        // The whole packet size (not the payload)
        double packetSize @unit(B);
        // The duration between bursts in which no packets are sent 
        sleepDuration = burstInterval;
        // Time between packets during bursts according packetSize and burstRatePerSecond
        sendInterval = replaceUnit(dropUnit(burstDuration)/(dropUnit(burstRatePerSecond)*dropUnit(burstDuration)/8.0/dropUnit(packetSize)),"s");
}
