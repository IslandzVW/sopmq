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
                return node_ptr();
            }
            
            auto upperIter = _ringByRange.upper_bound(key); //find the node after the primary
            upperIter--; //find the primary
            
            BOOST_ASSERT(upperIter != _ringByRange.end());
            
            return upperIter->second;
        }
        
    }
}