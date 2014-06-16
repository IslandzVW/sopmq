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

#include "cluster_endpoint.h"

namespace bc = boost::chrono;

namespace sopmq {
    namespace client {
        
        cluster_endpoint::cluster_endpoint(const shared::net::endpoint& ep)
        : _endpoint(ep), _isFailed(false)
        {
            
        }
        
        cluster_endpoint::~cluster_endpoint()
        {
            
        }
        
        bool cluster_endpoint::ready_for_retry() const
        {
            auto duration = bc::steady_clock::now() - _lastFailure;
            auto diffSecs = bc::duration_cast<bc::seconds>(duration);
            
            return diffSecs.count() >= _currentBackoff;
        }
        
        void cluster_endpoint::mark_failed()
        {
            _lastFailure = bc::steady_clock::now();
            _isFailed = true;
        }
        
        void cluster_endpoint::mark_up()
        {
            _isFailed = false;
            _currentBackoff = MINIMUM_BACKOFF_SECS;
        }
        
        const shared::net::endpoint& cluster_endpoint::network_endpoint() const
        {
            return _endpoint;
        }
        
    }
}