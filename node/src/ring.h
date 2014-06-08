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

#ifndef __Project__ring__
#define __Project__ring__

#include <map>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/noncopyable.hpp>
#include <array>

#include "node.h"
#include "endpoint.h"

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
            void add_node(node_ptr node);
            
            ///
            /// Finds the node that we believe to be primary for the given key
            ///
            node_ptr find_primary_node_for_key(boost::multiprecision::uint128_t key) const;
            
            ///
            /// Finds the primary and secondary nodes for the given key
            ///
            std::array<node_ptr, 3> find_nodes_for_key(boost::multiprecision::uint128_t key) const;
            
        private:
            typedef std::map<boost::multiprecision::uint128_t, node_ptr>::const_iterator const_ring_iterator;
            
            ///
            /// Map to the ring, sorted by the range start of each node
            ///
            std::map<boost::multiprecision::uint128_t, node_ptr> _ringByRange;
            
            ///
            /// Map to the nodes of the ring by their endpoints
            ///
            std::map<shared::net::endpoint, node_ptr> _ringByEndpoint;
            
            ///
            /// Finds the location of the secondary node on the ring
            ///
            const_ring_iterator find_secondary_node(boost::multiprecision::uint128_t key) const;
            
            ///
            /// Finds the location of the primary node on the ring
            ///
            const_ring_iterator find_primary_node(const_ring_iterator secondaryIter) const;
            
            ///
            /// Finds the location of the tertiary node on the ring given the iter to the secondary
            ///
            const_ring_iterator find_tertiary_node(const_ring_iterator secondaryIter) const;
        };
        
    }
}

#endif /* defined(__Project__ring__) */
