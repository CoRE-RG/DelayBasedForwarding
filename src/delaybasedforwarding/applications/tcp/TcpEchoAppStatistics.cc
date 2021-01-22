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

#include "TcpEchoAppStatistics.h"

namespace delaybasedforwarding {

Define_Module(TcpEchoAppStatistics);

Define_Module(TcpEchoAppThreadStatistics);

simsignal_t TcpEchoAppStatistics::rxBytesRateSignal = registerSignal("rxBytesRate");

void TcpEchoAppStatistics::captureBytesAtRate(inet::Packet *msg) {
    bytesRcvdRate += msg->getByteLength();
    int intervalls = simTime() / captureRate;
    if (intervalls > intervalsPassed) {
        emit(rxBytesRateSignal,bytesRcvdRate);
        bytesRcvdRate = 0;
        intervalsPassed=intervalls;
    }

}

void TcpEchoAppThreadStatistics::dataArrived(inet::Packet *msg, bool urgent) {
    TcpEchoAppStatistics *echoAppStatisticsModule = dynamic_cast<TcpEchoAppStatistics*>(echoAppModule);
    echoAppStatisticsModule->captureBytesAtRate(msg);
    TcpEchoAppThread::dataArrived(msg,urgent);
}

} //namespace
