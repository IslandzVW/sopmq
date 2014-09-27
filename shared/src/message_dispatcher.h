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
               rawname = os.path.splitext(os.path.basename(fn))[0]
               cog.outl("void dispatch(%s_ptr %s);" % (rawname,first_lower(rawname)))
             ]]]*/
            void dispatch(AnswerChallengeMessage_ptr answerChallengeMessage);
            void dispatch(AuthAckMessage_ptr authAckMessage);
            void dispatch(ChallengeResponseMessage_ptr challengeResponseMessage);
            void dispatch(GetChallengeMessage_ptr getChallengeMessage);
            void dispatch(Identifier_ptr identifier);
            void dispatch(NodeClock_ptr nodeClock);
            void dispatch(PublishMessage_ptr publishMessage);
            void dispatch(PublishResponseMessage_ptr publishResponseMessage);
            void dispatch(SubscribeQueueMessage_ptr subscribeQueueMessage);
            void dispatch(SubscribeResponseMessage_ptr subscribeResponseMessage);
            void dispatch(VectorClock_ptr vectorClock);
            //[[[end]]]
            
        public:
            /*[[[cog
             for fn in fnames:
               rawname = os.path.splitext(os.path.basename(fn))[0]
               cog.outl("void set_handler(std::function<void(%s_ptr)> handler);" % rawname)
             ]]]*/
            void set_handler(std::function<void(AnswerChallengeMessage_ptr)> handler);
            void set_handler(std::function<void(AuthAckMessage_ptr)> handler);
            void set_handler(std::function<void(ChallengeResponseMessage_ptr)> handler);
            void set_handler(std::function<void(GetChallengeMessage_ptr)> handler);
            void set_handler(std::function<void(Identifier_ptr)> handler);
            void set_handler(std::function<void(NodeClock_ptr)> handler);
            void set_handler(std::function<void(PublishMessage_ptr)> handler);
            void set_handler(std::function<void(PublishResponseMessage_ptr)> handler);
            void set_handler(std::function<void(SubscribeQueueMessage_ptr)> handler);
            void set_handler(std::function<void(SubscribeResponseMessage_ptr)> handler);
            void set_handler(std::function<void(VectorClock_ptr)> handler);
            //[[[end]]]
            
        private:
            std::function<void(Message_ptr, const std::string&)> _unhandledHandler;
            
            /*[[[cog
             for fn in fnames:
               rawname = os.path.splitext(os.path.basename(fn))[0]
               cog.outl("std::function<void(%s_ptr)> _%sHandler;" % (rawname,first_lower(rawname)))
             ]]]*/
            std::function<void(AnswerChallengeMessage_ptr)> _answerChallengeMessageHandler;
            std::function<void(AuthAckMessage_ptr)> _authAckMessageHandler;
            std::function<void(ChallengeResponseMessage_ptr)> _challengeResponseMessageHandler;
            std::function<void(GetChallengeMessage_ptr)> _getChallengeMessageHandler;
            std::function<void(Identifier_ptr)> _identifierHandler;
            std::function<void(NodeClock_ptr)> _nodeClockHandler;
            std::function<void(PublishMessage_ptr)> _publishMessageHandler;
            std::function<void(PublishResponseMessage_ptr)> _publishResponseMessageHandler;
            std::function<void(SubscribeQueueMessage_ptr)> _subscribeQueueMessageHandler;
            std::function<void(SubscribeResponseMessage_ptr)> _subscribeResponseMessageHandler;
            std::function<void(VectorClock_ptr)> _vectorClockHandler;
            //[[[end]]]
            
            ///
            /// Template function to execute the given handler if it is available, or
            /// the unhandled handler if it is not
            ///
            template <typename handler, typename message>
            void do_dispatch(handler h, message m)
            {
                if (h)
                {
                    h(m);
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
