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
            message_dispatcher(std::function<void(Message_ptr, const std::string&)> unhandledHandler);
            virtual ~message_dispatcher();
            
            /*[[[cog
             for fn in fnames:
               if "Message" in fn:
                 rawname = os.path.splitext(os.path.basename(fn))[0]
                 cog.outl("void dispatch(%s_ptr %s);" % (rawname,first_lower(rawname)))
             ]]]*/
            void dispatch(AnswerChallengeMessage_ptr answerChallengeMessage);
            void dispatch(AuthAckMessage_ptr authAckMessage);
            void dispatch(ChallengeResponseMessage_ptr challengeResponseMessage);
            void dispatch(ConsumeFromQueueMessage_ptr consumeFromQueueMessage);
            void dispatch(ConsumeResponseMessage_ptr consumeResponseMessage);
            void dispatch(GetChallengeMessage_ptr getChallengeMessage);
            void dispatch(GossipMessage_ptr gossipMessage);
            void dispatch(PublishMessage_ptr publishMessage);
            void dispatch(PublishResponseMessage_ptr publishResponseMessage);
            //[[[end]]]
            
        public:
            /*[[[cog
             for fn in fnames:
               if "Message" in fn:
                 rawname = os.path.splitext(os.path.basename(fn))[0]
                 cog.outl("void set_handler(std::function<void(%s_ptr)> handler);" % rawname)
                 cog.outl("void set_handler(std::function<void(%s_ptr)> handler, std::uint32_t inReplyTo);" % rawname)
                 cog.outl("")
             ]]]*/
            void set_handler(std::function<void(AnswerChallengeMessage_ptr)> handler);
            void set_handler(std::function<void(AnswerChallengeMessage_ptr)> handler, std::uint32_t inReplyTo);

            void set_handler(std::function<void(AuthAckMessage_ptr)> handler);
            void set_handler(std::function<void(AuthAckMessage_ptr)> handler, std::uint32_t inReplyTo);

            void set_handler(std::function<void(ChallengeResponseMessage_ptr)> handler);
            void set_handler(std::function<void(ChallengeResponseMessage_ptr)> handler, std::uint32_t inReplyTo);

            void set_handler(std::function<void(ConsumeFromQueueMessage_ptr)> handler);
            void set_handler(std::function<void(ConsumeFromQueueMessage_ptr)> handler, std::uint32_t inReplyTo);

            void set_handler(std::function<void(ConsumeResponseMessage_ptr)> handler);
            void set_handler(std::function<void(ConsumeResponseMessage_ptr)> handler, std::uint32_t inReplyTo);

            void set_handler(std::function<void(GetChallengeMessage_ptr)> handler);
            void set_handler(std::function<void(GetChallengeMessage_ptr)> handler, std::uint32_t inReplyTo);

            void set_handler(std::function<void(GossipMessage_ptr)> handler);
            void set_handler(std::function<void(GossipMessage_ptr)> handler, std::uint32_t inReplyTo);

            void set_handler(std::function<void(PublishMessage_ptr)> handler);
            void set_handler(std::function<void(PublishMessage_ptr)> handler, std::uint32_t inReplyTo);

            void set_handler(std::function<void(PublishResponseMessage_ptr)> handler);
            void set_handler(std::function<void(PublishResponseMessage_ptr)> handler, std::uint32_t inReplyTo);

            //[[[end]]]
            
        private:
            std::function<void(Message_ptr, const std::string&)> _unhandledHandler;
            
            /*[[[cog
             for fn in fnames:
               if "Message" in fn:
                 rawname = os.path.splitext(os.path.basename(fn))[0]
                 cog.outl("std::unordered_map<std::uint32_t, std::function<void(%s_ptr)>> _%sHandlers;" % (rawname,first_lower(rawname)))
             ]]]*/
            std::unordered_map<std::uint32_t, std::function<void(AnswerChallengeMessage_ptr)>> _answerChallengeMessageHandlers;
            std::unordered_map<std::uint32_t, std::function<void(AuthAckMessage_ptr)>> _authAckMessageHandlers;
            std::unordered_map<std::uint32_t, std::function<void(ChallengeResponseMessage_ptr)>> _challengeResponseMessageHandlers;
            std::unordered_map<std::uint32_t, std::function<void(ConsumeFromQueueMessage_ptr)>> _consumeFromQueueMessageHandlers;
            std::unordered_map<std::uint32_t, std::function<void(ConsumeResponseMessage_ptr)>> _consumeResponseMessageHandlers;
            std::unordered_map<std::uint32_t, std::function<void(GetChallengeMessage_ptr)>> _getChallengeMessageHandlers;
            std::unordered_map<std::uint32_t, std::function<void(GossipMessage_ptr)>> _gossipMessageHandlers;
            std::unordered_map<std::uint32_t, std::function<void(PublishMessage_ptr)>> _publishMessageHandlers;
            std::unordered_map<std::uint32_t, std::function<void(PublishResponseMessage_ptr)>> _publishResponseMessageHandlers;
            //[[[end]]]
            
            ///
            /// Template function to execute the given handler if it is available, or
            /// the unhandled handler if it is not
            ///
            template <typename hashmap, typename message>
            void do_dispatch(hashmap h, message m)
            {
                auto id = m->identity().in_reply_to();
                auto iter = h.find(id);
                
                if (iter != h.end())
                {
                    iter->second(m);
                    
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
