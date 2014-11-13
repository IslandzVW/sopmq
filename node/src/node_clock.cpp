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

#include "node_clock.h"

#include "comparison_error.h"

namespace sopmq {
    namespace node {
        
        void node_clock::to_protobuf(NodeClock* pbclock) const
        {
            pbclock->set_node_id(this->node_id);
            pbclock->set_generation(this->generation);
            pbclock->set_clock(this->clock);
        }
        
        bool operator ==(const node_clock& lhs, const node_clock& rhs)
        {
            return  lhs.node_id == rhs.node_id &&
            lhs.generation == rhs.generation &&
            lhs.clock == rhs.clock;
        }
        
        bool operator !=(const node_clock& lhs, const node_clock& rhs)
        {
            return !(lhs == rhs);
        }
        
        bool operator <(const node_clock& lhs, const node_clock& rhs)
        {
            if (lhs.node_id != rhs.node_id)
            {
                throw comparison_error("< and > not valid for node_clocks on different servers");
            }
            
            if (lhs.generation < rhs.generation ||
                (lhs.generation == rhs.generation && lhs.clock < rhs.clock))
            {
                return true;
            }
            
            return false;
        }
        
        bool operator >(const node_clock& lhs, const node_clock& rhs)
        {
            return (!(lhs < rhs)) && (lhs != rhs);
        }
        
    }
}