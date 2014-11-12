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


#ifndef __sopmq__inode_operations__
#define __sopmq__inode_operations__

#include "message_ptrs.h"
#include "operation_result.h"

#include <functional>
#include <memory>

namespace sopmq {
    namespace node {
        namespace intra {
            
            template <typename ReturnMessageType>
            struct return_message_callback_t
            {
                typedef std::function<void(operation_result<ReturnMessageType>&)> type;
            };
            
            ///
            /// Interface to intranode operations. Used to simplify the implementation of
            /// executing operations between nodes on the ring
            ///
            class inode_operations
            {
            public:
                typedef std::unique_ptr<inode_operations> ptr;
                
            public:
                
                ///
                /// Sends a gossip message to this node and registers for a callback when the node returns it
                ///
                virtual void send_gossip(GossipMessage_ptr message,
                                         return_message_callback_t<GossipMessage_ptr>::type responseCallback) = 0;
                
                ///
                /// Sends a proxy publish message to this node and registers for a callback when the status is available
                ///
                virtual void send_proxy_publish(PublishMessage_ptr clientMessage,
                                                return_message_callback_t<ProxyPublishResponseMessage_ptr>::type responseCallback) = 0;
                
                inode_operations();
                virtual ~inode_operations();
            };
            
        }
    }
}

#endif /* defined(__sopmq__inode_comms__) */
