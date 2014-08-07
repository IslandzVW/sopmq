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

#ifndef __Project__cluster_endpoint__
#define __Project__cluster_endpoint__

#include <boost/chrono/system_clocks.hpp>
#include "endpoint.h"

#include <memory>

namespace sopmq {
    namespace client {
        
        ///
        /// An extension of a normal endpoint that tracks node status with
        /// uptime and downtime
        ///
        class cluster_endpoint
        {
        public:
            typedef std::shared_ptr<cluster_endpoint> ptr;
            
        public:
            cluster_endpoint(const shared::net::endpoint& ep);
            virtual ~cluster_endpoint();
            
            ///
            /// Whether or not this endpoint is currently considered failed
            ///
            bool is_failed() const;
            
            ///
            /// Whether or not we're ready for a retry on this endpoint
            ///
            bool ready_for_retry() const;
            
            ///
            /// The number of seconds since the last time we failed
            ///
            boost::chrono::seconds secs_since_last_try() const;
            
            ///
            /// The number of seconds remaining until we should retry the connection
            ///
            boost::chrono::seconds secs_remining_before_retry() const;
            
            ///
            /// Marks this endpoint as failed
            ///
            void mark_failed();
            
            ///
            /// Marks this endpoint as up
            ///
            void mark_up();
            
            ///
            /// Returns the network endpoint
            ///
            const shared::net::endpoint& network_endpoint() const;
            
            
        private:
            ///
            /// The minimum time we back off a connection when there is
            /// a failure
            ///
            static const int MINIMUM_BACKOFF_SECS;
            
            ///
            /// The maximum amount of time we set for the next retry
            ///
            static const int MAXIMUM_BACKOFF_SECS;
            
            
            ///
            /// Sets the backoff to MINIMUM_BACKOFF_SECS if it is currently 0
            /// or doubles it if not up to MAXIMUM_BACKOFF_SECS
            ///
            void increase_backoff();
            
            ///
            /// The network endpoint
            ///
            shared::net::endpoint _endpoint;
            
            ///
            /// The last time we tried using this endpoint
            ///
            boost::chrono::steady_clock::time_point _lastFailure;
            
            ///
            /// The current backoff in seconds
            ///
            boost::chrono::seconds _currentBackoff;
            
            ///
            /// Is this endpoint failed?
            ///
            bool _isFailed;
        };
        
    }
}

#endif /* defined(__Project__cluster_endpoint__) */
