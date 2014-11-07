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

#include "local_node_operations.h"

#include <stdexcept>

namespace sopmq {
    namespace node {
        namespace intra {
            
            local_node_operations::local_node_operations()
            {
                
            }
            
            local_node_operations::~local_node_operations()
            {
                
            }
            
            void local_node_operations::send_gossip(GossipMessage_ptr message,
                                                    return_message_callback_t<GossipMessage_ptr>::type responseCallback)
            {
                throw std::logic_error("Refusing to gossip to self");
            }
            
            void local_node_operations::send_proxy_publish(PublishMessage_ptr clientMessage,
                                                           return_message_callback_t<ProxyPublishResponseMessage_ptr>::type responseCallback)
            {
                
            }
            
        }
    }
}