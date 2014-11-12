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

#ifndef __sopmq__local_node_operations__
#define __sopmq__local_node_operations__

#include "inode_operations.h"
#include "queue_manager.h"
#include "node_clock.h"
#include "ring.h"

namespace sopmq {
    namespace node {
        
        class node;
        
        namespace intra {
            
            ///
            /// Executes operations on the local node
            ///
            class local_node_operations : public inode_operations
            {
            public:
                local_node_operations(ring& ring, ::sopmq::node::node& node, node_clock& clock);
                virtual ~local_node_operations();
                
                ///
                /// Gossiping to ourselves doesnt make any sense. This method throws.
                ///
                virtual void send_gossip(GossipMessage_ptr message,
                                         return_message_callback_t<GossipMessage_ptr>::type responseCallback);
                
                virtual void send_proxy_publish(PublishMessage_ptr clientMessage,
                                                return_message_callback_t<ProxyPublishResponseMessage_ptr>::type responseCallback);
                
            private:
                ring& _ring;
                node& _node;
                node_clock& _clock;
                queue_manager3 _queue_manager;
            };
            
        }
    }
}

#endif /* defined(__sopmq__local_node_operations__) */
