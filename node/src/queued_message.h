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

#ifndef __sopmq__queued_message__
#define __sopmq__queued_message__

#include "vector_clock.h"

#include <boost/uuid/uuid.hpp>
#include <boost/chrono.hpp>

#include <array>
#include <cstdint>
#include <memory>
#include <unordered_map>

namespace sopmq {
    namespace node {

        ///
        /// A message that is queued for delivery to a subscriber
        /// \tparam RF The replication factor for this message
        ///
        template <size_t RF>
        class queued_message
        {
        public:
            typedef std::shared_ptr<queued_message<RF>> ptr;

        public:
            ///
            /// Constructs a new queued message from the data pointer and
            /// takes ownership of the data
            ///
            queued_message(boost::uuids::uuid id, std::string* data)
                : _id(id), _data(std::move(*data)), _local_time(boost::chrono::steady_clock::now())
            {
            }

            ///
            /// Sets the vector clock value for this message
            ///
            void set_vclock(vclock_t clock)
            {
                _vclock = clock;
            }


            ///
            /// Returns the unique message id for this message
            ///
            const boost::uuids::uuid& id() const
            {
                return _id;
            }

            ///
            /// Returns the payload for this message
            ///
            const std::string& data() const
            {
                return _data;
            }

            ///
            /// Returns the vector clock for this message
            ///
            const typename vclock_t<RF>::type& vector_clock() const
            {
                return _vclock;
            }

            ///
            /// The machine local time this message was received
            ///
            const boost::chrono::steady_clock::time_point& local_time() const
            {
                return _local_time;
            }


        private:
            boost::uuids::uuid _id;
            std::string _data;
            boost::chrono::steady_clock::time_point _local_time;
            typename vclock_t<RF>::type _vclock;
        };

        ///
        /// Message map for queued_message by id.
        /// \tparam RF Replication factor
        ///
        template <size_t RF>
        struct message_map_t
        {
            typedef std::unordered_map<boost::uuids::uuid, queued_message<RF>> type;
        };
    }
}

#endif /* defined(__sopmq__queued_message__) */
