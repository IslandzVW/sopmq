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

#include "logging.h"
#include "netutil.h"

using sopmq::error::network_error;

using namespace std::placeholders;

namespace sopmq {
    namespace util {
        
        void messageutil::read_message(boost::asio::io_service& ioService,
                                       boost::asio::ip::tcp::socket& socket,
                                        message_callback callback,
                                       uint32_t maxSize)
        {
            message_context ctx;
            ctx.callback = callback;
            ctx.max_message_size = maxSize;
            
            //read the message type
            netutil::read_u16(ioService, socket, std::bind(&messageutil::after_read_message_type,
                                                           std::ref(ioService), std::ref(socket),
                                                           ctx,
                                                           _1, _2));
        }
        
        void messageutil::after_read_message_type(boost::asio::io_service& ioService,
                                                  boost::asio::ip::tcp::socket& socket,
                                                  message_context ctx,
                                                  uint16_t messageType,
                                                  const boost::system::error_code& error)
        {
            if (error)
            {
                ctx.callback(nullptr, sopmq::messages::MT_INVALID, network_error(error.message()));
                return;
            }
            
            //validate the message
            if (messageType <= sopmq::messages::MT_INVALID || messageType >= sopmq::messages::MT_INVALID_OUT_OF_RANGE)
            {
                ctx.callback(nullptr, sopmq::messages::MT_INVALID, network_error("Message type was invalid"));
                return;
            }
            
            ctx.type = (sopmq::messages::message_type)messageType;
            
            //read the message size
            netutil::read_u32(ioService, socket, std::bind(&messageutil::after_read_message_size,
                                                           std::ref(ioService), std::ref(socket),
                                                           ctx,
                                                           _1, _2));
        }
        
        void messageutil::after_read_message_size(boost::asio::io_service& ioService,
                                                  boost::asio::ip::tcp::socket& socket,
                                                  message_context ctx,
                                                  uint32_t messageSize,
                                                  const boost::system::error_code& error)
        {
            if (error)
            {
                ctx.callback(nullptr, sopmq::messages::MT_INVALID, network_error(error.message()));
                return;
            }
            
            //sanity check the size
            if (messageSize > ctx.max_message_size)
            {
                LOG_SRC(error) << "message is too large (" << messageSize / 1024 << " MB)";
                ctx.callback(nullptr, sopmq::messages::MT_INVALID, network_error("Message was too large"));
                return;
            }
            
            //alloc
            boost::shared_array<char> buffer(new char[messageSize]);
            ctx.message_buffer = buffer;
            
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
                                                     message_context ctx,
                                                     const boost::system::error_code& error,
                                                     std::size_t bytes_transferred)
        {
            if (error)
            {
                ctx.callback(nullptr, sopmq::messages::MT_INVALID, network_error(error.message()));
                return;
            }
            
            //we have a message, decode it
            
        }
    }
}