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

package delaybasedforwarding.applications.tcp;
import inet.applications.tcpapp.TcpEchoApp;
import inet.applications.tcpapp.TcpEchoAppThread;

//
// The TcpEchoAppStatistics module is used to collect statistics about the received Bits per second
//
// @author Mehmet Cakir
//
simple TcpEchoAppStatistics extends TcpEchoApp
{
    parameters:
        @class(TcpEchoAppStatistics);
        // Signal emitted when capture rate is reached
        @signal[rxBitsRate](type=long);
        // Captures the bits according the capture rate
        @statistic[throughput](title="Rate over time"; source=rxBitsRate; unit=bps; record=stats,histogram?,vector; interpolationmode=none);
        // Custom serverThreadModuleType
        serverThreadModuleType = default("delaybasedforwarding.applications.tcp.TcpEchoAppThreadStatistics");
}

//
// The TcpEchoAppThreadStatistics module is the custom thread implementation for the TcpEchoAppStatistics module
//
simple TcpEchoAppThreadStatistics extends TcpEchoAppThread
{
    parameters:
        @class(TcpEchoAppThreadStatistics);
}
