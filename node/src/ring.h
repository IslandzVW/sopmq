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

#ifndef __sopmq__ring__
#define __sopmq__ring__

#include "node.h"
#include "endpoint.h"
#include "uint128.h"

#include <map>
#include <unordered_map>
#include <boost/noncopyable.hpp>
#include <array>


namespace sopmq {
    namespace node {
        
        ///
        /// Represents the ring as we currently understand it
        ///
        class ring : public boost::noncopyable
        {
        public:
            ring();
            virtual ~ring();
            
            ///
            /// Adds a new node to the ring
            ///
            void add_node(node::ptr node);
            
            ///
            /// Finds the node that we believe to be primary for the given key
            ///
            node::ptr find_primary_node_for_key(uint128 key) const;
            
            ///
            /// Finds the primary and secondary nodes for the given key
            ///
            std::array<node::ptr, 3> find_nodes_for_key(uint128 key) const;
            
        private:
            typedef std::map<uint128, node::ptr>::const_iterator const_ring_iterator;
            
            ///
            /// Map to the ring, sorted by the range start of each node
            ///
            std::map<uint128, node::ptr> _ring_by_range;
            
            ///
            /// Map between the node ID and the node
            ///
            std::unordered_map<std::uint32_t, node::ptr> _nodes_by_id;
            
            ///
            /// Finds the location of the secondary node on the ring
            ///
            const_ring_iterator find_secondary_node(uint128 key) const;
            
            ///
            /// Finds the location of the primary node on the ring
            ///
            const_ring_iterator find_primary_node(const_ring_iterator secondaryIter) const;
            
            ///
            /// Finds the location of the tertiary node on the ring given the iter to the secondary
            ///
            const_ring_iterator find_tertiary_node(const_ring_iterator secondaryIter) const;
        
            ///
            /// Checks to make sure adding this node doesn't conflict with any other node
            ///
            void check_no_conflict(node::ptr newNode) const;
        };
        
    }
}

#endif /* defined(__sopmq__ring__) */
