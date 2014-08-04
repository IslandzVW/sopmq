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

#include "messageutil.h"

#include <boost/make_shared.hpp>

#include "logging.h"
#include "netutil.h"

#include "GetChallengeMessage.pb.h"

using sopmq::error::network_error;

using namespace std::placeholders;
using namespace sopmq::util;

namespace sopmq {
    namespace message {
        
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
                case MT_GET_CHALLENGE:
                    messageutil::template_dispatch(ctx, std::make_shared<GetChallengeMessage>());
                    break;
                    
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
                                        network_error_callback errorCallback)
        {
            //boost::asio::async_write
        }
    }
}