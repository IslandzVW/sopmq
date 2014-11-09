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

#ifndef __sopmq__queue_manager__
#define __sopmq__queue_manager__

#include "message_queue.h"
#include "uint128.h"

#include <boost/heap/fibonacci_heap.hpp>

#include <thread>
#include <unordered_map>
#include <tuple>

namespace sopmq {
    namespace node {
        
        ///
        /// Manages all the queues in memory on a node
        ///
        template <size_t RF>
        class queue_manager
        {
        public:
            typedef message_queue<RF> message_queueX;
            
        public:
            queue_manager()
            {
                
            }
            
            virtual ~queue_manager()
            {
                
            }
            
            ///
            /// Returns a queue for the queue id
            ///
            message_queueX& get_queue(const uint128& queueId)
            {
                std::lock_guard<std::mutex> lock(_list_lock);
                
                auto qiter = _queues.find(queueId);
                if (qiter == _queues.end())
                {
                    queue_tuple_t& queue = _queues.emplace(queue_tuple_t(message_queueX(), expiry_heap_t::handle_type())).first;
                    auto handle = _queues_by_expiration.push(std::get<1>(queue));
                    std::get<0>(queue) = handle;
                }
                else
                {
                    return qiter->second;
                }
            }
            
            
            
        private:
            typedef boost::heap::fibonacci_heap<message_queueX*> expiry_heap_t;
            typedef std::tuple<message_queueX, typename expiry_heap_t::handle_type> queue_tuple_t;
            
            std::mutex _list_lock;
            
            std::unordered_map<uint128, queue_tuple_t> _queues;
            expiry_heap_t _queues_by_expiration;
        };
        
        typedef queue_manager<3> queue_manager3;
    }
}

#endif /* defined(__sopmq__queue_manager__) */
