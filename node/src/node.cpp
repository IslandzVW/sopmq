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

#include "settings.h"
#include "gossiper.h"

namespace bc = boost::chrono;

using sopmq::node::settings;

namespace sopmq {
    namespace node {
        
        node::node(std::uint32_t nodeId, uint128 rangeStart,
                   shared::net::endpoint endPoint)
        : _node_id(nodeId), _range_start(rangeStart), _endpoint(endPoint),
        _failure_detector(settings::instance().phiFailureThreshold, bc::milliseconds(gossiper::GOSSIP_INTERVAL_MS))
        {
            
        }
        
        node::~node()
        {
            
        }
        
        uint32_t node::node_id() const
        {
            return _node_id;
        }
        
        uint128 node::range_start() const
        {
            return _range_start;
        }
        
        shared::net::endpoint node::endpoint() const
        {
            return _endpoint;
        }
        
        bool node::is_alive() const
        {
            if (is_self()) return true;

            return _failure_detector.interpret() == failure_detector::UP;
        }
        
        bool node::is_self() const
        {
            return _node_id == settings::instance().nodeId;
        }
    }
}