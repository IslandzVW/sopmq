/*
 * SOPMQ - Scalable optionally persistent message queue
 * Copyright 2014 InWorldz, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "node.h"

namespace bc = boost::chrono;
namespace bmp = boost::multiprecision;

namespace sopmq {
    namespace node {
        
        node::node(bmp::uint128_t rangeStart, net::endpoint endPoint)
        : _rangeStart(rangeStart), _endpoint(endPoint)
        {
            
        }
        
        node::~node()
        {
            
        }
        
        bmp::uint128_t node::range_start() const
        {
            return _rangeStart;
        }
        
        net::endpoint node::endpoint() const
        {
            return _endpoint;
        }
        
        bool node::is_alive() const
        {
            auto duration = bc::steady_clock::now() - _lastHeartbeat;
            auto diffSecs = bc::duration_cast<bc::seconds>(duration);
            
            if (diffSecs.count() > HEARTBEAT_TIMEOUT_SECS)
            {
                return false;
            }
            
            return true;
        }
    }
}