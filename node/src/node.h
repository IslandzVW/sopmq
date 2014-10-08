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

#ifndef __sopmq__node__
#define __sopmq__node__

#include "endpoint.h"
#include "uint128.h"
#include "failure_detector.h"

#include <memory>
#include <boost/noncopyable.hpp>
#include <boost/chrono.hpp>

#include <string>
#include <cstdint>


namespace sopmq {
    namespace node {
        
        ///
        /// Represents a node that we're aware of in our ring that can service
        /// requests. This could be the local node, or a remote node
        ///
        class node : public boost::noncopyable
        {
        public:
            node(std::uint32_t nodeId, uint128 rangeStart,
                 shared::net::endpoint endPoint);
            
            virtual ~node();
            
            ///
            /// Returns the unique identifier for this node
            ///
            uint32_t node_id() const;
            
            ///
            /// Returns the beginning of the range we handle
            ///
            uint128 range_start() const;
            
            ///
            /// Returns the endpoint to reach this node
            ///
            shared::net::endpoint endpoint() const;
            
            ///
            /// Whether or not this node is known to be up
            ///
            bool is_alive() const;
            
            ///
            /// Whether or not this node represents the current executing node
            ///
            bool is_self() const;
            
            
        private:
            ///
            /// The unique identifier for this node
            ///
            std::uint32_t _node_id;
            
            ///
            /// The beginning of the range that we handle
            ///
            uint128 _range_start;
            
            ///
            /// The endpoint to contact this node at
            ///
            shared::net::endpoint _endpoint;
            
            ///
            /// Failure detector for this node
            ///
            failure_detector _failure_detector;
        };
        
        typedef std::shared_ptr<node> node_ptr;
    }
}

#endif /* defined(__sopmq__node__) */
