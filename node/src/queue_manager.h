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
#include "vector_clock.h"
#include "uint128.h"
#include "movable_noncopyable.h"

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
        class queue_manager : public shared::movable_noncopyable
        {
        public:
            typedef message_queue<RF> message_queueX;
            typedef vector_clock<RF> vector_clockX;
            
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
                    typename expiry_heap_t::handle_type t;
                    auto iter = _queues.emplace(queueId, queue_tuple_t(message_queueX(queueId), t));
                    queue_tuple_t& val = iter.first->second;
                    message_queueX& queue = std::get<0>(val);
                    auto handle = _queues_by_expiration.push(&queue);
                    std::get<1>(val) = handle;
                    
                    return queue;
                }
                else
                {
                    return std::get<0>(qiter->second);
                }
            }
            
            ///
            /// Enqueues the given message
            ///
            void enqueue_message(const uint128& queueId, const boost::uuids::uuid& messageId,
                                 std::string* data, uint32_t ttlSecs)
            {
                auto& queue = this->get_queue(queueId);
                
                queue.enqueue(messageId, data, ttlSecs);
            }
            
            
            ///
            /// Stamps the given message with a vector clock
            ///
            void stamp_message(const uint128& queueId, const boost::uuids::uuid& messageId,
                               const vector_clockX& clock)
            {
                auto& queue = this->get_queue(queueId);
                queue.stamp(messageId, clock);
            }
            
            
        private:
            typedef boost::heap::fibonacci_heap<message_queueX*> expiry_heap_t;
            typedef std::pair<message_queueX, typename expiry_heap_t::handle_type> queue_tuple_t;
            typedef std::unordered_map<uint128, queue_tuple_t> queue_map_t;
            
            std::mutex _list_lock;
            
            queue_map_t _queues;
            expiry_heap_t _queues_by_expiration;
        };
        
        typedef queue_manager<3> queue_manager3;
    }
}

#endif /* defined(__sopmq__queue_manager__) */
