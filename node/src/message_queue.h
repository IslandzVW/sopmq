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

#include <string>
#include <cstdint>
#include <boost/chrono.hpp>

namespace sopmq {
    namespace node {

        ///
        /// Queue for incoming messages from producers
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
                : _queue_id(queueId), _created_on(boost::chrono::steady_clock::now()), _clock(0)
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
            /// \brief Places the message into the unstamped collection
            /// Places the message into the unstamped collection and takes ownership of the data
            ///
            void enqueue(boost::uuids::uuid id, std::string* data)
            {
                _unstamped_messages.emplace(id, std::move(*data));
            }

            ///
            ///
            ///
            ///
            void stamp(boost::uuids::uuid id, typename vclock_t<RF>::type vclock)
            {
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
            /// Our current clock value for message vector clocks
            ///
            std::uint64_t _clock;

            ///
            /// The last time a message was received for this queue
            ///
            boost::chrono::steady_clock::time_point _last_message_received;

            ///
            /// Messages that have been put on the queue, but haven't been assigned
            /// a vector clock yet
            ///
            typename message_map_t<RF>::type _unstamped_messages;
        };
    }
}

#endif