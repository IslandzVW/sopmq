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
#include "inode_operations.h"
#include "node_clock.h"

#include <memory>
#include <boost/noncopyable.hpp>
#include <boost/chrono.hpp>

#include <string>
#include <cstdint>


namespace sopmq {
    namespace node {
        
        class ring;
        
        ///
        /// Represents a node that we're aware of in our ring that can service
        /// requests. This could be the local node, or a remote node
        ///
        class node : public boost::noncopyable
        {
        public:
            typedef std::shared_ptr<node> ptr;
            
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
            
            ///
            /// Indicates that this node has received a heartbeat and we should update
            /// the failure detector
            ///
            void heartbeat();
            
            ///
            /// Indicates we have actively seen a definite failure and this node should
            /// be marked down until we see the next heartbeat
            ///
            void set_failed();
            
            ///
            /// Returns an interface to operations that can be performed on this node
            ///
            intra::inode_operations& operations();
            
            ///
            /// Returns a reference to the self "this" node
            ///
            static node::ptr get_self();
            
            ///
            /// Creates inode_operations to perform tasks on the local node
            ///
            void init_local_operations(ring& ring);
            
            
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
            
            ///
            /// The handler for operations sent to this node
            ///
            intra::inode_operations::ptr _operations_handler;
            
            ///
            /// Whether or not this node has been forced into down status
            ///
            bool _forced_failure;
            
            ///
            /// The clock for this node
            ///
            node_clock _clock;
        };
    }
}

#endif /* defined(__sopmq__node__) */
