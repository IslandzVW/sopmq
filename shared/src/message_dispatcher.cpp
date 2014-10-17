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

#include "message_dispatcher.h"

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

/*[[[cog
 for fn in fnames:
   if "Message" in fn:
     rawname = os.path.splitext(os.path.basename(fn))[0]
     cog.outl("#include \"%s.pb.h\"" % rawname)
 ]]]*/
#include "AnswerChallengeMessage.pb.h"
#include "AuthAckMessage.pb.h"
#include "ChallengeResponseMessage.pb.h"
#include "ConsumeFromQueueMessage.pb.h"
#include "ConsumeResponseMessage.pb.h"
#include "GetChallengeMessage.pb.h"
#include "GossipMessage.pb.h"
#include "PublishMessage.pb.h"
#include "PublishResponseMessage.pb.h"
//[[[end]]]

namespace sopmq {
    namespace message {
        
        message_dispatcher::message_dispatcher()
        {
            
        }
        
        message_dispatcher::message_dispatcher(std::function<void(Message_ptr, const std::string&)> unhandledHandler)
        : _unhandledHandler(unhandledHandler)
        {
            
        }
        
        message_dispatcher::~message_dispatcher()
        {
            
        }
        
        void message_dispatcher::set_unhandled_handler(std::function<void (Message_ptr, const std::string &)> unhandledHandler)
        {
            _unhandledHandler = unhandledHandler;
        }
        
        /*[[[cog
         for fn in fnames:
           if "Message" in fn:
             rawname = os.path.splitext(os.path.basename(fn))[0]
             cog.outl("")
             cog.outl("void message_dispatcher::dispatch(%s_ptr %s)" % (rawname,first_lower(rawname)))
             cog.outl("{")
             cog.outl("    do_dispatch(_%sHandlers, %s);" % (first_lower(rawname), first_lower(rawname)))
             cog.outl("}")
             cog.outl("")
         ]]]*/

        void message_dispatcher::dispatch(AnswerChallengeMessage_ptr answerChallengeMessage)
        {
            do_dispatch(_answerChallengeMessageHandlers, answerChallengeMessage);
        }


        void message_dispatcher::dispatch(AuthAckMessage_ptr authAckMessage)
        {
            do_dispatch(_authAckMessageHandlers, authAckMessage);
        }


        void message_dispatcher::dispatch(ChallengeResponseMessage_ptr challengeResponseMessage)
        {
            do_dispatch(_challengeResponseMessageHandlers, challengeResponseMessage);
        }


        void message_dispatcher::dispatch(ConsumeFromQueueMessage_ptr consumeFromQueueMessage)
        {
            do_dispatch(_consumeFromQueueMessageHandlers, consumeFromQueueMessage);
        }


        void message_dispatcher::dispatch(ConsumeResponseMessage_ptr consumeResponseMessage)
        {
            do_dispatch(_consumeResponseMessageHandlers, consumeResponseMessage);
        }


        void message_dispatcher::dispatch(GetChallengeMessage_ptr getChallengeMessage)
        {
            do_dispatch(_getChallengeMessageHandlers, getChallengeMessage);
        }


        void message_dispatcher::dispatch(GossipMessage_ptr gossipMessage)
        {
            do_dispatch(_gossipMessageHandlers, gossipMessage);
        }


        void message_dispatcher::dispatch(PublishMessage_ptr publishMessage)
        {
            do_dispatch(_publishMessageHandlers, publishMessage);
        }


        void message_dispatcher::dispatch(PublishResponseMessage_ptr publishResponseMessage)
        {
            do_dispatch(_publishResponseMessageHandlers, publishResponseMessage);
        }

        //[[[end]]]
        
        /*[[[cog
         for fn in fnames:
           if "Message" in fn:
             rawname = os.path.splitext(os.path.basename(fn))[0]
             cog.outl("")
             cog.outl("void message_dispatcher::set_handler(std::function<void(%s_ptr)> handler)" % rawname)
             cog.outl("{")
             cog.outl("    _%sHandlers[0] = handler;" % first_lower(rawname))
             cog.outl("}")
             cog.outl("")
             cog.outl("void message_dispatcher::set_handler(std::function<void(%s_ptr)> handler, std::uint32_t inReplyTo)" % rawname)
             cog.outl("{")
             cog.outl("    _%sHandlers[inReplyTo] = handler;" % first_lower(rawname))
             cog.outl("}")
             cog.outl("")
             cog.outl("")
         ]]]*/

        void message_dispatcher::set_handler(std::function<void(AnswerChallengeMessage_ptr)> handler)
        {
            _answerChallengeMessageHandlers[0] = handler;
        }

        void message_dispatcher::set_handler(std::function<void(AnswerChallengeMessage_ptr)> handler, std::uint32_t inReplyTo)
        {
            _answerChallengeMessageHandlers[inReplyTo] = handler;
        }



        void message_dispatcher::set_handler(std::function<void(AuthAckMessage_ptr)> handler)
        {
            _authAckMessageHandlers[0] = handler;
        }

        void message_dispatcher::set_handler(std::function<void(AuthAckMessage_ptr)> handler, std::uint32_t inReplyTo)
        {
            _authAckMessageHandlers[inReplyTo] = handler;
        }



        void message_dispatcher::set_handler(std::function<void(ChallengeResponseMessage_ptr)> handler)
        {
            _challengeResponseMessageHandlers[0] = handler;
        }

        void message_dispatcher::set_handler(std::function<void(ChallengeResponseMessage_ptr)> handler, std::uint32_t inReplyTo)
        {
            _challengeResponseMessageHandlers[inReplyTo] = handler;
        }



        void message_dispatcher::set_handler(std::function<void(ConsumeFromQueueMessage_ptr)> handler)
        {
            _consumeFromQueueMessageHandlers[0] = handler;
        }

        void message_dispatcher::set_handler(std::function<void(ConsumeFromQueueMessage_ptr)> handler, std::uint32_t inReplyTo)
        {
            _consumeFromQueueMessageHandlers[inReplyTo] = handler;
        }



        void message_dispatcher::set_handler(std::function<void(ConsumeResponseMessage_ptr)> handler)
        {
            _consumeResponseMessageHandlers[0] = handler;
        }

        void message_dispatcher::set_handler(std::function<void(ConsumeResponseMessage_ptr)> handler, std::uint32_t inReplyTo)
        {
            _consumeResponseMessageHandlers[inReplyTo] = handler;
        }



        void message_dispatcher::set_handler(std::function<void(GetChallengeMessage_ptr)> handler)
        {
            _getChallengeMessageHandlers[0] = handler;
        }

        void message_dispatcher::set_handler(std::function<void(GetChallengeMessage_ptr)> handler, std::uint32_t inReplyTo)
        {
            _getChallengeMessageHandlers[inReplyTo] = handler;
        }



        void message_dispatcher::set_handler(std::function<void(GossipMessage_ptr)> handler)
        {
            _gossipMessageHandlers[0] = handler;
        }

        void message_dispatcher::set_handler(std::function<void(GossipMessage_ptr)> handler, std::uint32_t inReplyTo)
        {
            _gossipMessageHandlers[inReplyTo] = handler;
        }



        void message_dispatcher::set_handler(std::function<void(PublishMessage_ptr)> handler)
        {
            _publishMessageHandlers[0] = handler;
        }

        void message_dispatcher::set_handler(std::function<void(PublishMessage_ptr)> handler, std::uint32_t inReplyTo)
        {
            _publishMessageHandlers[inReplyTo] = handler;
        }



        void message_dispatcher::set_handler(std::function<void(PublishResponseMessage_ptr)> handler)
        {
            _publishResponseMessageHandlers[0] = handler;
        }

        void message_dispatcher::set_handler(std::function<void(PublishResponseMessage_ptr)> handler, std::uint32_t inReplyTo)
        {
            _publishResponseMessageHandlers[inReplyTo] = handler;
        }


        //[[[end]]]
    }
}

