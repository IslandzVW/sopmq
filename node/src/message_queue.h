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

#ifndef __sopmq__message_queue__
#define __sopmq__message_queue__

#include "queued_message.h"
#include "message_not_found_error.h"
#include "vector_clock.h"

#include <boost/lexical_cast.hpp>
#include <boost/chrono.hpp>
#include <boost/functional/hash.hpp>

#include <string>
#include <cstdint>
#include <unordered_map>
#include <map>
#include <vector>

namespace sopmq {
    namespace node {

        ///
        /// Message map for queued_message by id.
        /// \tparam RF Replication factor
        ///
        template <size_t RF>
        struct message_map_t
        {
            typedef std::unordered_map<boost::uuids::uuid, queued_message<RF>, boost::hash<boost::uuids::uuid>> type;
        };

        ///
        /// Priority queue for queued_message sorted by vclock
        /// \tparam RF Replication factor
        ///
        template <size_t RF>
        struct message_queue_t
        {
            typedef std::multimap<vector_clock<RF>, typename queued_message<RF>::ptr> type;
        };
        
        ///
        /// Map between a message ID and its handle
        /// \tparam RF Replication factor
        ///
        template <size_t RF>
        struct message_index_t
        {
            typedef std::unordered_map<boost::uuids::uuid, typename message_queue_t<RF>::type::iterator, boost::hash<boost::uuids::uuid>> type;
        };

        ///
        /// \brief Queue for incoming messages from producers
        ///
        template <size_t RF>
        class message_queue
        {
        public:
            ///
            /// \brief CTOR
            /// \param queueId The hex representation of the murmur hash of this queues name
            ///
            message_queue(const std::string& queueId)
                : _queue_id(queueId), _created_on(boost::chrono::steady_clock::now())
            {

            }

            ///
            /// \brief Returns the Hex representation of the Murmur hash for this queue name
            ///
            const std::string& queue_id() const
            {
                return _queue_id;
            }

            ///
            /// Places the message into the unstamped collection and takes ownership of the data
            /// \brief Places the message into the unstamped collection
			/// \param id The unique ID of this message
			/// \param data The binary payload for the message
			/// \param ttlSecs The number of seconds this message should live in the queue
            ///
            void enqueue(boost::uuids::uuid id, std::string* data, uint32_t ttlSecs)
            {
				if (! _ttl_set)
				{
					_ttl_set = true;
					_ttl = ttlSecs;
				}
                
                _unstamped_messages.insert(typename message_map_t<RF>::type::value_type(id, queued_message<RF>(id, data)));
            }
            
            ///
            /// \brief Sets the vector clock for the given message
            /// \param id The id of the message to set the clock on
            /// \param vclock The clock to set on the message
            /// \return Whether or not the message was found to set the stamp
            ///
            bool stamp(boost::uuids::uuid id, vector_clock<RF> vclock)
            {
                typedef typename message_map_t<RF>::type::iterator IterType;
                
                auto iter = _unstamped_messages.find(id);
                if (iter != _unstamped_messages.end())
                {
					auto message = std::make_shared<queued_message<RF>>(std::move(iter->second));
					message->update_local_timestamp();

                    
                    //hint that this will probably be the element proceeding the last
                    typename message_queue_t<RF>::type::iterator pos;
                    if (_queued_messages.size() > 0)
                    {
                        pos = _queued_messages.insert(_queued_messages.end()--,
                                                      typename message_queue_t<RF>::type::value_type(vclock, message));
                    }
                    else
                    {
                        pos = _queued_messages.insert(typename message_queue_t<RF>::type::value_type(vclock, message));
                    }
                    
                    _message_index.insert( typename message_index_t<RF>::type::value_type(id, pos) );
                    _unstamped_messages.erase(iter);
                    
                    return true;
                }
                else
                {
					return false;
                }
            }

			///
			/// \brief Expires messages that are beyond their TTL
			///
			void expire_messages()
			{
                auto now = boost::chrono::steady_clock::now();
                auto ttlSecs = boost::chrono::seconds(_ttl);
                
                while (!_queued_messages.empty() &&
                       now - _queued_messages.top().local_time() > ttlSecs)
                {
                    boost::uuids::uuid id = _queued_messages.top().id();
                    _queued_messages.pop();
                    _message_index.erase(id);
                }
                
                //also check unstamped for expirations
                std::vector<boost::uuids::uuid> expired;
                for (auto kvp : _unstamped_messages)
                {
                    if (kvp->value.local_time() > ttlSecs)
                    {
                        expired.push_back(kvp->key);
                    }
                }
                
                if (expired.size() > 0)
                {
                    for (auto id : expired)
                    {
                        _unstamped_messages.erase(id);
                    }
                }
			}

			///
			/// \brief The total memory size of all messages in this queue in bytes
			///
			uint32_t size()
			{
				return _total_message_size;
			}
            
            ///
            /// \brief Peeks messages greater than the given vclock
            ///
            std::vector<typename queued_message<RF>::ptr> peek(vector_clock<RF> lastMessage)
            {
                std::vector<typename queued_message<RF>::ptr> messages;
                
                auto start = _queued_messages.find(lastMessage);
                
                std::transform(start,
                               _queued_messages.end(),
                               std::back_inserter(messages),
                               [](const std::pair<vector_clock<RF>, typename queued_message<RF>::ptr>& p) { return p.second; });
                
                return messages;
            }
            
            ///
            /// \brief Peeks all messages
            ///
            std::vector<typename queued_message<RF>::ptr> peekAll()
            {
                std::vector<typename queued_message<RF>::ptr> messages;
                
                std::transform(_queued_messages.begin(),
                               _queued_messages.end(),
                               std::back_inserter(messages),
                               [](const std::pair<vector_clock<RF>, typename queued_message<RF>::ptr>& p) { return p.second; });
                
                return messages;
            }
            
            ///
            /// Removes a message from the queue
            ///
            void claim(boost::uuids::uuid messageId)
            {
                auto iter = _message_index.find(messageId);
                if (iter != _message_index.end())
                {
                    _queued_messages.erase(*iter);
                    _message_index.erase(messageId);
                }
            }
            
        private:
            ///
            /// Hex representation of the Murmur hash for this queue name
            ///
            std::string _queue_id;

            ///
            /// Time this queue was created
            /// 
            boost::chrono::steady_clock::time_point _created_on;

            ///
            /// The last time a message was received for this queue
            ///
            boost::chrono::steady_clock::time_point _last_message_received;

            ///
            /// Messages that have been put on the queue, but haven't been assigned
            /// a vector clock yet
            ///
            typename message_map_t<RF>::type _unstamped_messages;

            ///
            /// Messages that are actively in the queue and can be claimed
            ///
            typename message_queue_t<RF>::type _queued_messages;
            
            ///
            /// Message index from UUID to handle that can be used to reference
            /// messages in _queued_messages
            ///
            typename message_index_t<RF>::type _message_index;
            
			///
			/// Whether or not the TTL has been set yet
			///
			bool _ttl_set;

			///
			/// The TTL of all messages in this queue based on the TTL of the first message
			/// received
			///
			uint32_t _ttl;
			
			///
			/// The total size of all the messages in this queue
			///
			uint32_t _total_message_size;
        };

        ///
        /// Message queue def for RF = 3
        ///
        typedef message_queue<3> message_queue3;
    }
}

#endif