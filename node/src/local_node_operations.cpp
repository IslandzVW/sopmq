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

#include "PublishMessage.pb.h"
#include "ProxyPublishResponseMessage.pb.h"
#include "VectorClock.pb.h"

#include "node.h"
#include "util.h"
#include "messageutil.h"
#include "operation_result.h"

#include <stdexcept>
#include <memory>

using node = sopmq::node::node;

using namespace sopmq::shared;

namespace sopmq {
    namespace node {
        namespace intra {
            
            local_node_operations::local_node_operations(ring& ring, node& node, node_clock& clock)
            : _ring(ring), _node(node), _clock(clock)
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
                auto queueIdHash = util::murmur_hash3(clientMessage->queue_id());
                auto messageId = util::uuid_from_bytes(clientMessage->message_id());
                
                //clientMessage->mutable_content will be std::moved
                _queue_manager.enqueue_message(queueIdHash, messageId, clientMessage->mutable_content(), clientMessage->ttl());
                
                //update our component of the vector clock
                ++_clock.clock;
                
                //send the response back to the caller
                ProxyPublishResponseMessage_ptr response
                    = sopmq::message::messageutil::make_message<ProxyPublishResponseMessage>(0, clientMessage->identity().id());
                
                //share our knowlege of the clocks that handle this queue including ours that is now updated
                auto nodes = _ring.find_nodes_for_key(queueIdHash);
                
                std::unique_ptr<VectorClock> outClock(new VectorClock());
                for (auto node : nodes)
                {
                    node->clock().to_protobuf(outClock->add_clocks());
                }
                
                response->set_allocated_clock(outClock.get());
                outClock.release();
                
                operation_result<ProxyPublishResponseMessage_ptr> result(response);
                responseCallback(result);
            }
            
        }
    }
}