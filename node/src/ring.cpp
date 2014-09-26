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

#include "range_conflict_error.h"
#include "id_conflict_error.h"

#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>

#include <string>

using namespace std;

namespace bmp = boost::multiprecision;

using sopmq::error::range_conflict_error;
using sopmq::error::id_conflict_error;

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
            this->check_no_conflict(node);
            
            _ring_by_range.emplace(node->range_start(), node);
            _nodes_by_id.emplace(node->node_id(), node);
        }
        
        void ring::check_no_conflict(node_ptr newNode) const
        {
            auto idIter = _nodes_by_id.find(newNode->node_id());
            if (idIter != _nodes_by_id.end())
            {
                //id conflict
                throw id_conflict_error("the node id " +
                                        boost::lexical_cast<std::string>(newNode->node_id()) +
                                        " is already taken");
            }
            
            auto iter = _ring_by_range.find(newNode->range_start());
            if (iter != _ring_by_range.end())
            {
                //range conflict
                throw range_conflict_error("the range " +
                                           boost::lexical_cast<std::string>(newNode->range_start()) +
                                           " which node " +
                                           boost::lexical_cast<std::string>(newNode->node_id()) +
                                           " is proposing to handle " +
                                           " is already handled by node " +
                                           boost::lexical_cast<std::string>(iter->second->node_id()));
            }
        }
        
        node_ptr ring::find_primary_node_for_key(bmp::uint128_t key) const
        {
            if (_ring_by_range.empty())
            {
                return nullptr;
            }
            
            const_ring_iterator secondaryIter = this->find_secondary_node(key);
            const_ring_iterator primaryIter = this->find_primary_node(secondaryIter);
            
            BOOST_ASSERT(primaryIter != _ring_by_range.end());
            
            return primaryIter->second;
        }
        
        std::array<node_ptr, 3> ring::find_nodes_for_key(boost::multiprecision::uint128_t key) const
        {
            if (_ring_by_range.empty())
            {
                return std::array<node_ptr, 3>();
            }
            
            const_ring_iterator secondaryIter = this->find_secondary_node(key);
            const_ring_iterator primaryIter = this->find_primary_node(secondaryIter);
            const_ring_iterator tertiaryIter = this->find_tertiary_node(secondaryIter);
            
            BOOST_ASSERT(primaryIter != _ring_by_range.end());
            BOOST_ASSERT(secondaryIter != _ring_by_range.end());
            BOOST_ASSERT(tertiaryIter != _ring_by_range.end());
            
            auto ret = std::array<node_ptr, 3>();
            
			ret[0] = primaryIter->second;
			ret[1] = secondaryIter->second;
			ret[2] = tertiaryIter->second;

            return ret;
        }
        
        ring::const_ring_iterator ring::find_secondary_node(boost::multiprecision::uint128_t key) const
        {
            auto iter = _ring_by_range.upper_bound(key); //find the secondary node
            
            if (iter == _ring_by_range.end()) //wrap around
            {
                iter = _ring_by_range.begin();
            }
            
            return iter;
        }
        
        ring::const_ring_iterator ring::find_primary_node(const_ring_iterator secondaryIter) const
        {
            if (secondaryIter == _ring_by_range.begin()) //special case, we need to wrap to the end of the ring
            {
                secondaryIter = _ring_by_range.end();
            }
            
            secondaryIter--; //find the primary
            
            return secondaryIter;
        }
        
        ring::const_ring_iterator ring::find_tertiary_node(const_ring_iterator secondaryIter) const
        {
            secondaryIter++; //find the tertiary
            
            if (secondaryIter == _ring_by_range.end())
            {
                //wrap around
                secondaryIter = _ring_by_range.begin();
            }
            
            return secondaryIter;
        }
    }
}