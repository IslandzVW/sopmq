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

#ifndef __sopmq__message_dispatcher__
#define __sopmq__message_dispatcher__

#include "message_ptrs.h"
#include "network_operation_result.h"

#include <functional>
#include <memory>
#include <unordered_map>
#include <cstdint>

//
// ATTENTION: This file uses cog (http://nedbatchelder.com/code/cog/) for code
// generation. Do not edit the generated chunks between cog and end tags
//


/*[[[cog
import cog
import glob
import os

ast = '*'

first_lower = lambda s: s[:1].lower() + s[1:] if s else ''
fnames = glob.glob("../messages/" + ast + ".proto")
]]]*/
//[[[end]]]

namespace sopmq {
    namespace message {
        
        ///
        /// Handles dispatch of network messages to selected functions
        ///
        class message_dispatcher
        {
        public:
            typedef std::shared_ptr<message_dispatcher> ptr;
            typedef std::weak_ptr<message_dispatcher> wptr;
            
        public:
            message_dispatcher();
            message_dispatcher(std::function<void(Message_ptr, const std::string&)> unhandledHandler);
            virtual ~message_dispatcher();
            
            ///
            /// Cancels all callbacks waiting on a dispatch with the given error
            ///
            void cancel_all_with_error(const sopmq::shared::net::network_operation_result& result);
            
            /*[[[cog
             for fn in fnames:
               if "Message" in fn:
                 rawname = os.path.splitext(os.path.basename(fn))[0]
                 cog.outl("void dispatch(const sopmq::shared::net::network_operation_result& result, %s_ptr %s);" % (rawname,first_lower(rawname)))
             ]]]*/
            void dispatch(const sopmq::shared::net::network_operation_result& result, AnswerChallengeMessage_ptr answerChallengeMessage);
            void dispatch(const sopmq::shared::net::network_operation_result& result, AuthAckMessage_ptr authAckMessage);
            void dispatch(const sopmq::shared::net::network_operation_result& result, ChallengeResponseMessage_ptr challengeResponseMessage);
            void dispatch(const sopmq::shared::net::network_operation_result& result, ConsumeFromQueueMessage_ptr consumeFromQueueMessage);
            void dispatch(const sopmq::shared::net::network_operation_result& result, ConsumeResponseMessage_ptr consumeResponseMessage);
            void dispatch(const sopmq::shared::net::network_operation_result& result, GetChallengeMessage_ptr getChallengeMessage);
            void dispatch(const sopmq::shared::net::network_operation_result& result, GossipMessage_ptr gossipMessage);
            void dispatch(const sopmq::shared::net::network_operation_result& result, ProxyPublishMessage_ptr proxyPublishMessage);
            void dispatch(const sopmq::shared::net::network_operation_result& result, ProxyPublishResponseMessage_ptr proxyPublishResponseMessage);
            void dispatch(const sopmq::shared::net::network_operation_result& result, PublishMessage_ptr publishMessage);
            void dispatch(const sopmq::shared::net::network_operation_result& result, PublishResponseMessage_ptr publishResponseMessage);
            void dispatch(const sopmq::shared::net::network_operation_result& result, StampMessage_ptr stampMessage);
            //[[[end]]]
            
        public:
            void set_unhandled_handler(std::function<void(Message_ptr, const std::string&)> unhandledHandler);
            
            /*[[[cog
             for fn in fnames:
               if "Message" in fn:
                 rawname = os.path.splitext(os.path.basename(fn))[0]
                 cog.outl("void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, %s_ptr)> handler);" % rawname)
                 cog.outl("void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, %s_ptr)> handler, std::uint32_t inReplyTo);" % rawname)
                 cog.outl("")
             ]]]*/
            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, AnswerChallengeMessage_ptr)> handler);
            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, AnswerChallengeMessage_ptr)> handler, std::uint32_t inReplyTo);

            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, AuthAckMessage_ptr)> handler);
            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, AuthAckMessage_ptr)> handler, std::uint32_t inReplyTo);

            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, ChallengeResponseMessage_ptr)> handler);
            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, ChallengeResponseMessage_ptr)> handler, std::uint32_t inReplyTo);

            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, ConsumeFromQueueMessage_ptr)> handler);
            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, ConsumeFromQueueMessage_ptr)> handler, std::uint32_t inReplyTo);

            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, ConsumeResponseMessage_ptr)> handler);
            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, ConsumeResponseMessage_ptr)> handler, std::uint32_t inReplyTo);

            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, GetChallengeMessage_ptr)> handler);
            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, GetChallengeMessage_ptr)> handler, std::uint32_t inReplyTo);

            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, GossipMessage_ptr)> handler);
            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, GossipMessage_ptr)> handler, std::uint32_t inReplyTo);

            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, ProxyPublishMessage_ptr)> handler);
            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, ProxyPublishMessage_ptr)> handler, std::uint32_t inReplyTo);

            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, ProxyPublishResponseMessage_ptr)> handler);
            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, ProxyPublishResponseMessage_ptr)> handler, std::uint32_t inReplyTo);

            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, PublishMessage_ptr)> handler);
            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, PublishMessage_ptr)> handler, std::uint32_t inReplyTo);

            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, PublishResponseMessage_ptr)> handler);
            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, PublishResponseMessage_ptr)> handler, std::uint32_t inReplyTo);

            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, StampMessage_ptr)> handler);
            void set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, StampMessage_ptr)> handler, std::uint32_t inReplyTo);

            //[[[end]]]
            
        private:
            std::function<void(Message_ptr, const std::string&)> _unhandledHandler;
            
            /*[[[cog
             for fn in fnames:
               if "Message" in fn:
                 rawname = os.path.splitext(os.path.basename(fn))[0]
                 cog.outl("std::unordered_map<std::uint32_t, std::function<void(const sopmq::shared::net::network_operation_result&, %s_ptr)>> _%sHandlers;" % (rawname,first_lower(rawname)))
             ]]]*/
            std::unordered_map<std::uint32_t, std::function<void(const sopmq::shared::net::network_operation_result&, AnswerChallengeMessage_ptr)>> _answerChallengeMessageHandlers;
            std::unordered_map<std::uint32_t, std::function<void(const sopmq::shared::net::network_operation_result&, AuthAckMessage_ptr)>> _authAckMessageHandlers;
            std::unordered_map<std::uint32_t, std::function<void(const sopmq::shared::net::network_operation_result&, ChallengeResponseMessage_ptr)>> _challengeResponseMessageHandlers;
            std::unordered_map<std::uint32_t, std::function<void(const sopmq::shared::net::network_operation_result&, ConsumeFromQueueMessage_ptr)>> _consumeFromQueueMessageHandlers;
            std::unordered_map<std::uint32_t, std::function<void(const sopmq::shared::net::network_operation_result&, ConsumeResponseMessage_ptr)>> _consumeResponseMessageHandlers;
            std::unordered_map<std::uint32_t, std::function<void(const sopmq::shared::net::network_operation_result&, GetChallengeMessage_ptr)>> _getChallengeMessageHandlers;
            std::unordered_map<std::uint32_t, std::function<void(const sopmq::shared::net::network_operation_result&, GossipMessage_ptr)>> _gossipMessageHandlers;
            std::unordered_map<std::uint32_t, std::function<void(const sopmq::shared::net::network_operation_result&, ProxyPublishMessage_ptr)>> _proxyPublishMessageHandlers;
            std::unordered_map<std::uint32_t, std::function<void(const sopmq::shared::net::network_operation_result&, ProxyPublishResponseMessage_ptr)>> _proxyPublishResponseMessageHandlers;
            std::unordered_map<std::uint32_t, std::function<void(const sopmq::shared::net::network_operation_result&, PublishMessage_ptr)>> _publishMessageHandlers;
            std::unordered_map<std::uint32_t, std::function<void(const sopmq::shared::net::network_operation_result&, PublishResponseMessage_ptr)>> _publishResponseMessageHandlers;
            std::unordered_map<std::uint32_t, std::function<void(const sopmq::shared::net::network_operation_result&, StampMessage_ptr)>> _stampMessageHandlers;
            //[[[end]]]
            
            ///
            /// Template function to execute the given handler if it is available, or
            /// the unhandled handler if it is not
            ///
            template <typename hashmap, typename network_result, typename message>
            void do_dispatch(hashmap h, network_result r, message m)
            {
                auto id = m->identity().in_reply_to();
                auto iter = h.find(id);
                
                if (iter != h.end())
                {
                    iter->second(r, m);
                    
                    if (id != 0)
                    {
                        h.erase(iter);
                    }
                }
                else
                {
                    _unhandledHandler(std::static_pointer_cast<::google::protobuf::Message>(m), typeid(m).name());
                }
            }
        };
        
    }
}


#endif /* defined(__sopmq__message_dispatcher__) */
