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

#ifndef __DELAYBASEDFORWARDING_QUEUEING_SCHEDULER_DYNAMICPRIORITY_DBFDYNAMICPRIORITYSCHEDULER_H_
#define __DELAYBASEDFORWARDING_QUEUEING_SCHEDULER_DYNAMICPRIORITY_DBFDYNAMICPRIORITYSCHEDULER_H_

#include <omnetpp.h>
#include "delaybasedforwarding/queueing/scheduler/priority/DBFPriorityScheduler.h"

using namespace omnetpp;

namespace delaybasedforwarding {

/**
 * @brief The dynamic priority scheduler for delay based forwarded packets
 *
 * @ingroup delaybasedforwarding/queueing/scheduler
 *
 * @author Mehmet Mueller
 */

class DBFDynamicPriorityScheduler : public DBFPriorityScheduler
{
  /**
   * Methods
   */
  public:
  protected:
    virtual void handleMessage(cMessage *msg);
  private:

  /**
   * Member variables
   */
  public:
  protected:
  private:
};

} //namespace

#endif
