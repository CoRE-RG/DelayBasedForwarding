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

#include <delaybasedforwarding/applications/tcp/DBFTcpSessionApp.h>

namespace delaybasedforwarding {

Define_Module(DBFTcpSessionApp);

#define MSGKIND_SEND       2
#define MSGKIND_CLOSE      3

void DBFTcpSessionApp::sendData()
{
    if (tClose > simTime()) {
        long numBytes = sendBytes;
        EV_INFO << "sending data with " << numBytes << " bytes\n";
        sendPacket(createDataPacket(numBytes));
    }
}

void DBFTcpSessionApp::socketDataArrived(inet::TcpSocket *socket, inet::Packet *msg, bool urgent) {
    TcpSessionApp::socketDataArrived(socket, msg, urgent);
    if (tClose < simTime()) {
        timeoutMsg->setKind(MSGKIND_CLOSE);
        scheduleAt(simTime(), timeoutMsg);
    }

}

} //namespace
