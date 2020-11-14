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

#ifndef __DELAYBASEDFORWARDING_DBFTCPAPP_H_
#define __DELAYBASEDFORWARDING_DBFTCPAPP_H_

#include <omnetpp.h>
#include <inet/applications/tcpapp/TcpSessionApp.h>

using namespace omnetpp;

namespace delaybasedforwarding {

/**
 * @brief Extends the TcpSessionApp to send continuously packets with a given size
 *
 * @ingroup delaybasedforwarding/applications/tcp
 *
 * @author Mehmet Cakir
 */
class DBFTcpSessionApp : public inet::TcpSessionApp
{
  protected:
    virtual void sendData() override;
    virtual void socketDataArrived(inet::TcpSocket *socket, inet::Packet *msg, bool urgent) override;
};

} //namespace

#endif