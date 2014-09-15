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

#ifndef __sopmq__node_clock__
#define __sopmq__node_clock__

#include <cstdint>

namespace sopmq {
    namespace node {
        
        ///
        /// The value used for the current per-node state in vector clocks
        ///
        class node_clock
        {
        public:
            ///
            /// \brief The node's unique identifier
            ///
            std::uint32_t node_id;
            
            ///
            /// \brief The node's generation (number of restarts)
            ///
            std::uint32_t generation;
            
            ///
            /// \brief The node's message based clock.
            //// This is the number of messages received since the node restarted
            ///
            std::uint64_t clock;
        };
        
        bool operator ==(const node_clock& lhs, const node_clock& rhs);
        
        bool operator !=(const node_clock& lhs, const node_clock& rhs);
        
        bool operator <(const node_clock& lhs, const node_clock& rhs);
        
        bool operator >(const node_clock& lhs, const node_clock& rhs);
        
    }
}


#endif /* defined(__sopmq__node_clock__) */
