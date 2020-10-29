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

import inet.common.TagBase;

namespace delaybasedforwarding;

//
// Provides delay-based forwarding related paramaters for tagging
//
// @author Mehmet Cakir
//
class DBFHeaderTag extends inet::TagBase
{
    // Parameters to lookup
    simtime_t lDelay; // delay from predecessor node up to actual node
    simtime_t hDelay; // delay from actual node to next node
    simtime_t fromDelay; // sum of delay from origin node up to actual node
    simtime_t toDelay; // sum of delay from actual node up to destination node
    uint32_t fromHops; // sum of hops from origin node up to actual node
    uint32_t toHops; // sum of hops from actual node up to destination node
    string nexthop; // next node where the packet will be forwarded to
    
    // Parameters to calculate with
    uint32_t admit; // admit flag determines higher priority over non-admitted packets
    simtime_t tRcv; // time when packet was received at ingress
    simtime_t eDelay; // experienced delay
    simtime_t dMin; // minimum end-to-end delay required
    simtime_t dMax; // maximum end-to-end delay required
    
    // Parameters to calculate
    simtime_t lqBudgetMin; // minimum local latency queueing budget
    simtime_t lqBudgetMax; // maximum local latency queueing budget
    simtime_t tMin; // time when the packet should be sent at earliest
    simtime_t tMax; // time when the packet should be sent at latest
    
    // Utility parameters
    bool fromNetwork; // indicates if packet comes from network
}