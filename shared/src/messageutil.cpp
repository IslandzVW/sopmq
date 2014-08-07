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


//
// ATTENTION: This file uses cog (http://nedbatchelder.com/code/cog/) for code
// generation. Do not edit the generated chunks between cog and end tags
//


#include "messageutil.h"

#include "logging.h"
#include "netutil.h"

/*[[[cog
 import cog
 import glob
 import os
 
 ast = '*'
 
 first_lower = lambda s: s[:1].lower() + s[1:] if s else ''
 fnames = glob.glob("../messages/" + ast + ".proto")
 
 for fn in fnames:
    rawname = os.path.splitext(os.path.basename(fn))[0]
    cog.outl('#include "%s.pb.h"' % rawname)
 ]]]*/
#include "AnswerChallengeMessage.pb.h"
#include "AuthAckMessage.pb.h"
#include "ChallengeResponseMessage.pb.h"
#include "GetChallengeMessage.pb.h"
#include "Identifier.pb.h"
//[[[end]]]


#include <boost/make_shared.hpp>
#include <array>


using sopmq::error::network_error;

using namespace std::placeholders;
using namespace sopmq::util;
namespace ba = boost::asio;

namespace sopmq {
    namespace message {
        
        boost::pool<> messageutil::s_mem_pool(messageutil::HEADER_SIZE);
        
        void messageutil::read_message(boost::asio::io_service& ioService,
                                       boost::asio::ip::tcp::socket& socket,
                                       network_error_callback errorCallback,
                                       message_dispatcher& dispatcher,
                                       uint32_t maxSize)
        {
            message_context_ptr ctx(std::make_shared<message_context>(dispatcher));
            ctx->error_callback = errorCallback;
            ctx->max_message_size = maxSize;
            
            //read the message type
            netutil::read_u16(ioService, socket, std::bind(&messageutil::after_read_message_type,
                                                           std::ref(ioService), std::ref(socket),
                                                           ctx,
                                                           _1, _2));
        }
        
        void messageutil::after_read_message_type(boost::asio::io_service& ioService,
                                                  boost::asio::ip::tcp::socket& socket,
                                                  message_context_ptr ctx,
                                                  uint16_t messageType,
                                                  const boost::system::error_code& error)
        {
            if (error)
            {
                ctx->error_callback(network_error(error.message()));
                return;
            }
            
            //validate the message
            if (messageType <= sopmq::message::MT_INVALID || messageType >= sopmq::message::MT_INVALID_OUT_OF_RANGE)
            {
                ctx->error_callback(network_error("Message type was invalid"));
                return;
            }
            
            ctx->type = (sopmq::message::message_type)messageType;
            
            //read the message size
            netutil::read_u32(ioService, socket, std::bind(&messageutil::after_read_message_size,
                                                           std::ref(ioService), std::ref(socket),
                                                           ctx,
                                                           _1, _2));
        }
        
        void messageutil::after_read_message_size(boost::asio::io_service& ioService,
                                                  boost::asio::ip::tcp::socket& socket,
                                                  message_context_ptr ctx,
                                                  uint32_t messageSize,
                                                  const boost::system::error_code& error)
        {
            if (error)
            {
                ctx->error_callback(network_error(error.message()));
                return;
            }
            
            //sanity check the size
            if (messageSize > ctx->max_message_size)
            {
                LOG_SRC(error) << "message is too large (" << messageSize / 1024 << " MB)";
                ctx->error_callback(network_error("Message was too large"));
                return;
            }
            
            //alloc
            boost::shared_array<char> buffer(new char[messageSize]);
            ctx->message_buffer = buffer;
            
            boost::asio::async_read(socket,
                                    boost::asio::buffer(buffer.get(), messageSize),
                                    std::bind(&messageutil::after_read_message_content,
                                              std::ref(ioService),
                                              std::ref(socket),
                                              ctx,
                                              _1,
                                              _2));
        }
        
        void messageutil::after_read_message_content(boost::asio::io_service& ioService,
                                                     boost::asio::ip::tcp::socket& socket,
                                                     message_context_ptr ctx,
                                                     const boost::system::error_code& error,
                                                     std::size_t bytes_transferred)
        {
            if (error)
            {
                ctx->error_callback(network_error(error.message()));
                return;
            }
            
            BOOST_ASSERT(bytes_transferred == ctx->message_size);
            
            //we have a message, decode it
            messageutil::switch_dispatch(ctx);
        }
        
        void messageutil::switch_dispatch(message_context_ptr ctx)
        {
            switch (ctx->type)
            {
                /*[[[cog
                 def underscore(name):
                    import re
                    s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
                    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1).upper()
                 
                 for fn in fnames:
                    rawname = os.path.splitext(os.path.basename(fn))[0]
                    enumName = rawname
                    if enumName.endswith('Message'):
                        enumName = enumName[:-7]
                    else:
                        continue
                 
                 
                    enumName = underscore(enumName)
                 
                    cog.outl("case MT_%s:" % enumName)
                    cog.outl("    messageutil::template_dispatch(ctx, std::make_shared<%s>());" % rawname)
                    cog.outl("    break;");
                    cog.outl("");
                 ]]]*/
                case MT_ANSWER_CHALLENGE:
                    messageutil::template_dispatch(ctx, std::make_shared<AnswerChallengeMessage>());
                    break;

                case MT_AUTH_ACK:
                    messageutil::template_dispatch(ctx, std::make_shared<AuthAckMessage>());
                    break;

                case MT_CHALLENGE_RESPONSE:
                    messageutil::template_dispatch(ctx, std::make_shared<ChallengeResponseMessage>());
                    break;

                case MT_GET_CHALLENGE:
                    messageutil::template_dispatch(ctx, std::make_shared<GetChallengeMessage>());
                    break;

                //[[[end]]]
                    
                default:
                    throw std::runtime_error("messageutil::switch_dispatch() unhandled message type"
                                             + boost::lexical_cast<std::string>(ctx->type));
            }
        }
        
        Identifier* messageutil::build_id(std::uint32_t id, std::uint32_t inReplyTo)
        {
            auto newId = new Identifier();
            newId->set_id(id);
            newId->set_in_reply_to(inReplyTo);
            
            return newId;
        }
        
        void messageutil::write_message(sopmq::message::message_type type, Message_ptr message,
                                        boost::asio::io_service &ioService,
                                        boost::asio::ip::tcp::socket &socket,
                                        network_status_callback statusCallback)
        {
            send_context_ptr ctx(new send_context
                                 {
                                     std::unique_ptr<char[], void(*)(char*)>(static_cast<char*>(s_mem_pool.malloc()), &messageutil::free_mem),
                                     std::string(),
                                     statusCallback
                                 });
            
            message->SerializeToString(&ctx->messageBuf);
            
            auto netId = boost::asio::detail::socket_ops::host_to_network_short(type);
            auto netSize = boost::asio::detail::socket_ops::host_to_network_long(ctx->messageBuf.size());
            BOOST_STATIC_ASSERT(sizeof(netId) == 2);
            BOOST_STATIC_ASSERT(sizeof(netSize) == 4);
            
            BOOST_STATIC_ASSERT(sizeof(netId) + sizeof(netSize) == HEADER_SIZE);
            
            std::memcpy(ctx->headerBuf.get(), &netId, sizeof(netId));
            std::memcpy(ctx->headerBuf.get() + sizeof(netId), &netSize, sizeof(netSize));
            
            std::array<ba::const_buffer, 2> bufs = {
                {
                ba::buffer(ctx->headerBuf.get(), HEADER_SIZE),
                ba::buffer(ctx->messageBuf)
                }
            };
            
            ba::async_write(socket, bufs, std::bind(&messageutil::after_write_message,
                                                    ctx, _1, _2));
        }
        
        void messageutil::after_write_message(send_context_ptr ctx,
                                              const boost::system::error_code& error,
                                              size_t bytesTransferred)
        {
            if (error)
            {
                ctx->statusCallback(false, sopmq::error::network_error(error));
            }
            else
            {
                ctx->statusCallback(true, sopmq::error::network_error(error));
            }
        }
        
        void messageutil::free_mem(char *mem)
        {
            s_mem_pool.free(mem);
        }
    }
}