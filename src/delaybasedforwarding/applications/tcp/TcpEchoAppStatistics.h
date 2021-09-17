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

#ifndef __DELAYBASEDFORWARDING_TCPECHOAPPSTATISTICS_H_
#define __DELAYBASEDFORWARDING_TCPECHOAPPSTATISTICS_H_

#include <omnetpp.h>
#include <inet/applications/tcpapp/TcpEchoApp.h>

using namespace omnetpp;

namespace delaybasedforwarding {

/**
 * @brief Extended TcpEchoApp to capture received bits per second statistics
 *
 * @ingroup delaybasedforwarding/applications/tcp/
 *
 * @author Mehmet Cakir
 */
class TcpEchoAppStatistics : public inet::TcpEchoApp
{
  private:

    /**
     * @brief The rate of every capture of received bytes
     */
    simtime_t captureRate = SimTime(10,SimTimeUnit::SIMTIME_MS);

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;

    /**
     * @brief Counter for received bits
     */
    long bitsRcvdRate = 0;

    /**
     * @brief Captures the bytes according the captureRate
     *
     * @param msg    the received message whose bits are to be captured
     */
    void captureBytesAtRate(inet::Packet *msg);

    /**
     * @brief The signal that is emitted according the capture rate
     */
    static simsignal_t rxBitsRateSignal;

  public:
    friend class TcpEchoAppThreadStatistics;
};

/**
 * @brief Extended TcpEchoAppThread for the TcpEchoApp
 *
 * @ingroup delaybasedforwarding/applications/tcp/
 *
 * @author Mehmet Cakir
 */
class TcpEchoAppThreadStatistics : public inet::TcpEchoAppThread {
public:
    /**
     * @brief Initialization of the TcpEchoAppThreadStatistics module
     *
     * @param hostmodule    the TcpServerHostApp module
     * @param socket        the TcpSocket module
     */
    virtual void init(inet::TcpServerHostApp *hostmodule, inet::TcpSocket *socket) override {
        TcpServerThreadBase::init(hostmodule, socket); echoAppModule = check_and_cast<TcpEchoAppStatistics *>(hostmod);
    }

    /*
     * @brief Called when a data packet arrives
     *
     * @param msg    the arrived packet
     * @param urgent the urgent flag
     */
    virtual void dataArrived(inet::Packet *msg, bool urgent) override;

};

} //namespace

#endif
