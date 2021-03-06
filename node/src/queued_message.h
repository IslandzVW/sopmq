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
#include <vector>

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
            void set_vclock(vector_clock<RF>& mclock)
            {
                _vclock = mclock;
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
            const vector_clock<RF>& clock() const
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
            
            ///
            /// The amount of time that this message has existed on this machine
            ///
            const boost::chrono::steady_clock::duration age() const
            {
                return boost::chrono::steady_clock::now() - _local_time;
            }
            
			///
			/// Updates the local timestamp for the given message to NOW
			///
			void update_local_timestamp()
			{
				_local_time = boost::chrono::steady_clock::now();
			}

			///
			/// Returns the size of the message
			///
			uint32_t size()
			{
				return calc_size(_data.length());
			}
            
            static uint32_t calc_size(size_t dataSize)
            {
                return
                    sizeof(boost::uuids::uuid)
                    + sizeof(boost::chrono::steady_clock::time_point)
                    + sizeof(vector_clock<RF>)
                    + dataSize;
            }


        private:
            boost::uuids::uuid _id;
            std::string _data;
            boost::chrono::steady_clock::time_point _local_time;
            vector_clock<RF> _vclock;
        };
    }
}

#endif /* defined(__sopmq__queued_message__) */
