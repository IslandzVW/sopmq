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

#include "node_clock.h"
#include "comparison_error.h"
#include "VectorClock.pb.h"
#include "comparison_error.h"

#include <array>
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <type_traits>
#include <string>

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
            typedef std::array<node_clock, RF> vclock_t;
            
        public:
            vector_clock()
            : _value()
            {
                
            }
            
            ///
            /// Constructor to convert from a network VectorClock in a message to a server clock
            ///
            vector_clock(const VectorClock& netClock)
            {
                if (netClock.clocks_size() > RF)
                {
                    throw comparison_error("Network clock size of " + std::to_string(netClock.clocks_size()) +
                                           " is greater than RF of " + std::to_string(RF));
                }
                
                for (int i = 0; i < netClock.clocks_size(); ++i)
                {
                    const auto& clock = netClock.clocks(i);
                    
                    _value[i].node_id = clock.node_id();
                    _value[i].generation = clock.generation();
                    _value[i].clock = clock.clock();
                }
            }

            /*
            //this doesnt compile under vc11  :(
            template <std::size_t RF1 = RF, typename std::enable_if<RF1 == 3, int>::type = 0>
            vector_clock(const node_clock& n1, const node_clock& n2, const node_clock& n3)
                : _value{n1,n2,n3}
            {
            }
            */
            
            ///
            /// Returns the clock's value
            ///
            inline const vclock_t& value() const
            {
                return _value;
            }
            
            ///
            /// Sets the clock at the given position
            ///
            inline void set(size_t pos, const node_clock& clock)
            {
                _value[pos] = clock;
            }
            
            ///
            /// Returns a new vector clock that contains the highest value from
            /// the given clocks
            ///
            static vector_clock<RF> max(const vector_clock<RF>& a, const vector_clock<RF>& b)
            {
                //verify the clocks first
                for (size_t i = 0; i < RF; ++i)
                {
                    if (a.value()[i].node_id != b.value()[i].node_id)
                    {
                        throw comparison_error("max() not valid for vector clocks with different quorums");
                    }
                }
                
                vector_clock result;
                for (size_t i = 0; i < RF; ++i)
                {
                    result.set(i, std::max(a.value()[i], b.value()[i]));
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
            //we must be able to LT compare vector clocks with different
            //node ids. therefore, we only compare nodes with matching ids
            
            //normally, once we see a left clock less than right, we can consider
            //the left vector clock less. however, this function will also handle
            //invalid clocks by keeping tract of the number of lesser clock values.
            //we consider the left less than the right if it has a greater number
            //of clocks less than the right
            int numLeftLess = 0;
            int numRightLess = 0;
            
            // most of the time the values will be node aligned. we should try to take
            // advantage of this.
            
            for (std::size_t i = 0; i < RF; i++)
            {
                for (std::size_t j = 0; j < RF; j++)
                {
                    const node_clock& left = lhs.value()[i];
                    const node_clock& right = rhs.value()[j];
                    
                    if (left.node_id == right.node_id)
                    {
                        if (left < right) ++numLeftLess;
                        else if (right < left) ++numRightLess;
                        
                        break;
                    }
                }
            }

            return numLeftLess > numRightLess;
        }
        
        template <std::size_t RF>
        bool operator >(const vector_clock<RF>& lhs, const vector_clock<RF>& rhs)
        {
            return (!(lhs < rhs)) && (lhs != rhs);
        }

        ///
        /// Vector clock def for RF = 3
        ///
        typedef vector_clock<3> vector_clock3;
    }
}
#endif