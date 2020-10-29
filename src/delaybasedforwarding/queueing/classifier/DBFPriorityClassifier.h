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

#ifndef __DELAYBASEDFORWARDING_DBFPRIORITYCLASSIFIER_H_
#define __DELAYBASEDFORWARDING_DBFPRIORITYCLASSIFIER_H_

#include <omnetpp.h>
#include <inet/queueing/classifier/PriorityClassifier.h>
#include <map>

using namespace omnetpp;

namespace delaybasedforwarding {

/**
 * @brief The priority classifier for delay based forwarded packets
 *
 * @ingroup delaybasedforwarding/queueing/classifier
 *
 * @author Mehmet Cakir
 */

class DBFPriorityClassifier : public inet::queueing::PriorityClassifier
{

  private:
    /**
     * @brief Creates dynamically a delay-based forwarding queue
     *
     * @return The index of the queue in the consumers list of this classifier
     */
    int createDBFQueue();

    /**
     * @brief Returns index of priority queue for a given delta. If there is no queue available then it will be created.
     *
     * @param delta The given delta
     * @return The index of the priority queue for the given delta
     */
    int getIndexOfQueueForDelta(simtime_t delta);
  public:
    /**
     * @brief Returns the deltaQueueMap
     *
     * @return The deltaQueueMap
     */
    std::map<simtime_t, int> *getDeltaQueueMap();

  protected:
    /**
     * @brief The initialize method
     *
     * @param stage The stage of the initialization process
     */
    virtual void initialize(int stage) override;

    /**
     * @brief The handle message method
     *
     * @param msg A scheduled self message or received message
     */
    virtual void handleMessage(cMessage *msg) override;

    /**
     * @brief Classifies packets to determine in which queue the packet is pushed
     *
     * @param packet The given packet which is classified
     *
     * @return The index of the queue where the packet fits the best
     */
    virtual int classifyPacket(inet::Packet *packet) override;

  private:

    /**
     * @brief The delta steps used determining if new queue is needed
     */
    simtime_t deltaSteps;

    /**
     * @brief The maximum delta used determining if packet should be pushed in infinite queue
     */
    simtime_t maximumDelta;

    /**
     * @brief The delta queue map which contains the index of a delta queue in the consumer and provider list
     */
    std::map<simtime_t, int, std::less<simtime_t>> *deltaQueueMap;
};

} //namespace

#endif
