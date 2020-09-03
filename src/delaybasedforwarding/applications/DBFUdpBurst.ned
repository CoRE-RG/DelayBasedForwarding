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

simple DBFUdpBurst extends UdpBasicBurst {
    parameters:
        double burstInterval @unit(s);        // The interval the burst will periodically be started
        double burstRatePerSecond @unit(bps); // The burst rate per second in bit
        double packetSize @unit(B);           // The whole packet size (not the payload)
        sleepDuration = burstInterval;
        sendInterval = replaceUnit(dropUnit(burstDuration)/(dropUnit(burstRatePerSecond)*dropUnit(burstDuration)/8.0/dropUnit(packetSize)),"s");
}
