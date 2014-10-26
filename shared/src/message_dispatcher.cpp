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
        
        void message_dispatcher::cancel_all_with_error(const sopmq::shared::net::network_operation_result &result)
        {
            /*[[[cog
             for fn in fnames:
                if "Message" in fn:
                    rawname = os.path.splitext(os.path.basename(fn))[0]
                    cog.outl("for (auto handler : _%sHandlers)\n{" % first_lower(rawname))
                    cog.outl("  handler.second(result, nullptr);")
                    cog.outl("}\n");
             ]]]*/
            for (auto handler : _answerChallengeMessageHandlers)
            {
              handler.second(result, nullptr);
            }

            for (auto handler : _authAckMessageHandlers)
            {
              handler.second(result, nullptr);
            }

            for (auto handler : _challengeResponseMessageHandlers)
            {
              handler.second(result, nullptr);
            }

            for (auto handler : _consumeFromQueueMessageHandlers)
            {
              handler.second(result, nullptr);
            }

            for (auto handler : _consumeResponseMessageHandlers)
            {
              handler.second(result, nullptr);
            }

            for (auto handler : _getChallengeMessageHandlers)
            {
              handler.second(result, nullptr);
            }

            for (auto handler : _gossipMessageHandlers)
            {
              handler.second(result, nullptr);
            }

            for (auto handler : _publishMessageHandlers)
            {
              handler.second(result, nullptr);
            }

            for (auto handler : _publishResponseMessageHandlers)
            {
              handler.second(result, nullptr);
            }

            //[[[end]]]
        }
        
        /*[[[cog
         for fn in fnames:
           if "Message" in fn:
             rawname = os.path.splitext(os.path.basename(fn))[0]
             cog.outl("")
             cog.outl("void message_dispatcher::dispatch(const sopmq::shared::net::network_operation_result& result, %s_ptr %s)" % (rawname,first_lower(rawname)))
             cog.outl("{")
             cog.outl("    do_dispatch(_%sHandlers, result, %s);" % (first_lower(rawname), first_lower(rawname)))
             cog.outl("}")
             cog.outl("")
         ]]]*/

        void message_dispatcher::dispatch(const sopmq::shared::net::network_operation_result& result, AnswerChallengeMessage_ptr answerChallengeMessage)
        {
            do_dispatch(_answerChallengeMessageHandlers, result, answerChallengeMessage);
        }


        void message_dispatcher::dispatch(const sopmq::shared::net::network_operation_result& result, AuthAckMessage_ptr authAckMessage)
        {
            do_dispatch(_authAckMessageHandlers, result, authAckMessage);
        }


        void message_dispatcher::dispatch(const sopmq::shared::net::network_operation_result& result, ChallengeResponseMessage_ptr challengeResponseMessage)
        {
            do_dispatch(_challengeResponseMessageHandlers, result, challengeResponseMessage);
        }


        void message_dispatcher::dispatch(const sopmq::shared::net::network_operation_result& result, ConsumeFromQueueMessage_ptr consumeFromQueueMessage)
        {
            do_dispatch(_consumeFromQueueMessageHandlers, result, consumeFromQueueMessage);
        }


        void message_dispatcher::dispatch(const sopmq::shared::net::network_operation_result& result, ConsumeResponseMessage_ptr consumeResponseMessage)
        {
            do_dispatch(_consumeResponseMessageHandlers, result, consumeResponseMessage);
        }


        void message_dispatcher::dispatch(const sopmq::shared::net::network_operation_result& result, GetChallengeMessage_ptr getChallengeMessage)
        {
            do_dispatch(_getChallengeMessageHandlers, result, getChallengeMessage);
        }


        void message_dispatcher::dispatch(const sopmq::shared::net::network_operation_result& result, GossipMessage_ptr gossipMessage)
        {
            do_dispatch(_gossipMessageHandlers, result, gossipMessage);
        }


        void message_dispatcher::dispatch(const sopmq::shared::net::network_operation_result& result, PublishMessage_ptr publishMessage)
        {
            do_dispatch(_publishMessageHandlers, result, publishMessage);
        }


        void message_dispatcher::dispatch(const sopmq::shared::net::network_operation_result& result, PublishResponseMessage_ptr publishResponseMessage)
        {
            do_dispatch(_publishResponseMessageHandlers, result, publishResponseMessage);
        }

        //[[[end]]]
        
        /*[[[cog
         for fn in fnames:
           if "Message" in fn:
             rawname = os.path.splitext(os.path.basename(fn))[0]
             cog.outl("")
             cog.outl("void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, %s_ptr)> handler)" % rawname)
             cog.outl("{")
             cog.outl("    if (handler)")
             cog.outl("    {")
             cog.outl("        _%sHandlers[0] = handler;" % first_lower(rawname))
             cog.outl("    }")
             cog.outl("    else")
             cog.outl("    {")
             cog.outl("        _%sHandlers.erase(0);" % first_lower(rawname))
             cog.outl("    }")
             cog.outl("}")
             cog.outl("")
             cog.outl("void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, %s_ptr)> handler, std::uint32_t inReplyTo)" % rawname)
             cog.outl("{")
             cog.outl("    _%sHandlers[inReplyTo] = handler;" % first_lower(rawname))
             cog.outl("}")
             cog.outl("")
             cog.outl("")
         ]]]*/

        void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, AnswerChallengeMessage_ptr)> handler)
        {
            if (handler)
            {
                _answerChallengeMessageHandlers[0] = handler;
            }
            else
            {
                _answerChallengeMessageHandlers.erase(0);
            }
        }

        void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, AnswerChallengeMessage_ptr)> handler, std::uint32_t inReplyTo)
        {
            _answerChallengeMessageHandlers[inReplyTo] = handler;
        }



        void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, AuthAckMessage_ptr)> handler)
        {
            if (handler)
            {
                _authAckMessageHandlers[0] = handler;
            }
            else
            {
                _authAckMessageHandlers.erase(0);
            }
        }

        void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, AuthAckMessage_ptr)> handler, std::uint32_t inReplyTo)
        {
            _authAckMessageHandlers[inReplyTo] = handler;
        }



        void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, ChallengeResponseMessage_ptr)> handler)
        {
            if (handler)
            {
                _challengeResponseMessageHandlers[0] = handler;
            }
            else
            {
                _challengeResponseMessageHandlers.erase(0);
            }
        }

        void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, ChallengeResponseMessage_ptr)> handler, std::uint32_t inReplyTo)
        {
            _challengeResponseMessageHandlers[inReplyTo] = handler;
        }



        void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, ConsumeFromQueueMessage_ptr)> handler)
        {
            if (handler)
            {
                _consumeFromQueueMessageHandlers[0] = handler;
            }
            else
            {
                _consumeFromQueueMessageHandlers.erase(0);
            }
        }

        void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, ConsumeFromQueueMessage_ptr)> handler, std::uint32_t inReplyTo)
        {
            _consumeFromQueueMessageHandlers[inReplyTo] = handler;
        }



        void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, ConsumeResponseMessage_ptr)> handler)
        {
            if (handler)
            {
                _consumeResponseMessageHandlers[0] = handler;
            }
            else
            {
                _consumeResponseMessageHandlers.erase(0);
            }
        }

        void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, ConsumeResponseMessage_ptr)> handler, std::uint32_t inReplyTo)
        {
            _consumeResponseMessageHandlers[inReplyTo] = handler;
        }



        void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, GetChallengeMessage_ptr)> handler)
        {
            if (handler)
            {
                _getChallengeMessageHandlers[0] = handler;
            }
            else
            {
                _getChallengeMessageHandlers.erase(0);
            }
        }

        void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, GetChallengeMessage_ptr)> handler, std::uint32_t inReplyTo)
        {
            _getChallengeMessageHandlers[inReplyTo] = handler;
        }



        void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, GossipMessage_ptr)> handler)
        {
            if (handler)
            {
                _gossipMessageHandlers[0] = handler;
            }
            else
            {
                _gossipMessageHandlers.erase(0);
            }
        }

        void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, GossipMessage_ptr)> handler, std::uint32_t inReplyTo)
        {
            _gossipMessageHandlers[inReplyTo] = handler;
        }



        void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, PublishMessage_ptr)> handler)
        {
            if (handler)
            {
                _publishMessageHandlers[0] = handler;
            }
            else
            {
                _publishMessageHandlers.erase(0);
            }
        }

        void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, PublishMessage_ptr)> handler, std::uint32_t inReplyTo)
        {
            _publishMessageHandlers[inReplyTo] = handler;
        }



        void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, PublishResponseMessage_ptr)> handler)
        {
            if (handler)
            {
                _publishResponseMessageHandlers[0] = handler;
            }
            else
            {
                _publishResponseMessageHandlers.erase(0);
            }
        }

        void message_dispatcher::set_handler(std::function<void(const sopmq::shared::net::network_operation_result&, PublishResponseMessage_ptr)> handler, std::uint32_t inReplyTo)
        {
            _publishResponseMessageHandlers[inReplyTo] = handler;
        }


        //[[[end]]]
    }
}

