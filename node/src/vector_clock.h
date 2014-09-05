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

#ifndef __sopmq__vector_clock__
#define __sopmq__vector_clock__

#include <array>
#include <cstdint>
#include <cstddef>
#include <algorithm>

namespace sopmq {
    namespace node {

        ///
        /// Vector clock of the given replication factor
        /// \tparam RF The replication factor
        ///
        template <std::size_t RF>
        class vector_clock
        {
        public:
            typedef std::array<std::uint64_t, RF> vclock_t;
            
        public:
            vector_clock()
            : _value()
            {
                
            }
            
            ///
            /// Returns the clock's value
            ///
            inline vclock_t& value()
            {
                return _value;
            }
            
            ///
            /// Returns a new vector clock that contains the highest value from
            /// the given clocks
            ///
            static vector_clock max(const vector_clock& a, const vector_clock& b)
            {
                vector_clock result;
                for (size_t i = 0; i < RF; ++i)
                {
                    result.value()[i] = std::max(a.value()[i], b.value[i]);
                }
                
                return result;
            }
            
        private:
            vclock_t _value;
        };
        
        template <std::size_t RF>
        bool operator ==(const vector_clock<RF>& lhs, const vector_clock<RF>& rhs)
        {
            return lhs.value() == rhs.value();
        }
        
        template <std::size_t RF>
        bool operator !=(const vector_clock<RF>& lhs, const vector_clock<RF>& rhs)
        {
            return !(lhs == rhs);
        }

        template <std::size_t RF>
        bool operator <(const vector_clock<RF>& lhs, const vector_clock<RF>& rhs)
        {
            for (std::size_t i = 0; i < RF; i++)
            {
                if (lhs.value()[i] < rhs.value()[i])
                {
                    return true;
                }
            }

            return false;
        }
        
        template <std::size_t RF>
        bool operator >(const vector_clock<RF>& lhs, const vector_clock<RF>& rhs)
        {
            return (!(lhs < rhs)) && (lhs != rhs);
        }


    }
}
#endif