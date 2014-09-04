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

namespace sopmq {
    namespace node {

        ///
        /// Vector clock of the given replication factor
        /// \tparam RF The replication factor
        ///
        template <std::size_t RF>
        struct vclock_t
        {
            typedef std::array<std::uint64_t, RF> type;
        };

        template <std::size_t RF>
        bool operator <(const typename vclock_t<RF>::type& lhs, const typename vclock_t<RF>::type& rhs)
        {
            for (std::size_t i = 0; i < RF; i++)
            {
                if (lhs[i] < rhs[i])
                {
                    return true;
                }
            }

            return false;
        }
        
        template <std::size_t RF>
        bool operator >(const typename vclock_t<RF>::type& lhs, const typename vclock_t<RF>::type& rhs)
        {
            return (!(lhs < rhs)) && (lhs != rhs);
        }


    }
}
#endif