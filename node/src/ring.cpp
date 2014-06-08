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

#include "ring.h"

#include <boost/assert.hpp>

using namespace std;

namespace bmp = boost::multiprecision;

namespace sopmq {
    namespace node {
        
        ring::ring()
        {
            
        }
        
        ring::~ring()
        {
            
        }
        
        void ring::add_node(node_ptr node)
        {
            _ringByRange.insert(make_pair(node->range_start(), node));
            _ringByEndpoint.insert(make_pair(node->endpoint(), node));
        }
        
        node_ptr ring::find_primary_node_for_key(bmp::uint128_t key) const
        {
            if (_ringByRange.empty())
            {
                return nullptr;
            }
            
            const_ring_iterator secondaryIter = this->find_secondary_node(key);
            const_ring_iterator primaryIter = this->find_primary_node(secondaryIter);
            
            BOOST_ASSERT(primaryIter != _ringByRange.end());
            
            return primaryIter->second;
        }
        
        std::array<node_ptr, 3> ring::find_nodes_for_key(boost::multiprecision::uint128_t key) const
        {
            if (_ringByRange.empty())
            {
                return std::array<node_ptr, 3>{ {nullptr, nullptr, nullptr} };
            }
            
            const_ring_iterator secondaryIter = this->find_secondary_node(key);
            const_ring_iterator primaryIter = this->find_primary_node(secondaryIter);
            const_ring_iterator tertiaryIter = this->find_tertiary_node(secondaryIter);
            
            BOOST_ASSERT(primaryIter != _ringByRange.end());
            BOOST_ASSERT(secondaryIter != _ringByRange.end());
            BOOST_ASSERT(tertiaryIter != _ringByRange.end());
            
            auto ret = std::array<node_ptr, 3>{
                {
                    primaryIter->second,
                    secondaryIter->second,
                    tertiaryIter->second
                } };
            
            return ret;
        }
        
        ring::const_ring_iterator ring::find_secondary_node(boost::multiprecision::uint128_t key) const
        {
            auto iter = _ringByRange.upper_bound(key); //find the secondary node
            
            if (iter == _ringByRange.end()) //wrap around
            {
                iter = _ringByRange.begin();
            }
            
            return iter;
        }
        
        ring::const_ring_iterator ring::find_primary_node(const_ring_iterator secondaryIter) const
        {
            if (secondaryIter == _ringByRange.begin()) //special case, we need to wrap to the end of the ring
            {
                secondaryIter = _ringByRange.end();
            }
            
            secondaryIter--; //find the primary
            
            return secondaryIter;
        }
        
        ring::const_ring_iterator ring::find_tertiary_node(const_ring_iterator secondaryIter) const
        {
            secondaryIter++; //find the tertiary
            
            if (secondaryIter == _ringByRange.end())
            {
                //wrap around
                secondaryIter = _ringByRange.begin();
            }
            
            return secondaryIter;
        }
    }
}