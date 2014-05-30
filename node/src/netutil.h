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

#ifndef __Project__netutil__
#define __Project__netutil__

#include <functional>
#include <boost/asio.hpp>
#include <boost/pool/poolfwd.hpp>
#include <boost/shared_ptr.hpp>
#include <cstdint>

#include <google/protobuf/message.h>

#include "message_types.h"

namespace sopmq {
    namespace node {
        namespace util {
            
            struct message_context;
            
            class netutil
            {
            public:
                typedef std::function<void(boost::shared_ptr<::google::protobuf::Message>,
                                           sopmq::messages::message_type messageType,
                                           const boost::system::error_code& error)>
                message_callback;
                
                
                ///
                /// Reads an unsigned integer from the given socket
                ///
                static void read_u32(boost::asio::io_service& ioService,
                                     boost::asio::ip::tcp::socket& socket,
                                     std::function<void(uint32_t, const boost::system::error_code& error)> callback);
                
                ///
                /// Reads an unsigned short from the given socket
                ///
                static void read_u16(boost::asio::io_service& ioService,
                                     boost::asio::ip::tcp::socket& socket,
                                     std::function<void(uint16_t, const boost::system::error_code& error)> callback);
                
                ///
                /// Reads an unknown message type from the wire
                ///
                static void read_message(boost::asio::io_service& ioService,
                                         boost::asio::ip::tcp::socket& socket,
                                         message_callback callback);
                
            private:
                static boost::pool<> s_mem_pool;
                
                static void after_u32_read(char* buffer,
                                           std::function<void(uint32_t, const boost::system::error_code& error)> callback,
                                           const boost::system::error_code& error,
                                           std::size_t bytes_transferred);
                
                static void after_u16_read(char* buffer,
                                           std::function<void(uint16_t, const boost::system::error_code& error)> callback,
                                           const boost::system::error_code& error,
                                           std::size_t bytes_transferred);
                
                static void free_mem(char* mem);
                
                static void after_read_message_type(boost::asio::io_service& ioService,
                                                    boost::asio::ip::tcp::socket& socket,
                                                    message_context ctx,
                                                    uint16_t messageType,
                                                    const boost::system::error_code& error);
                
                static void after_read_message_size(boost::asio::io_service& ioService,
                                                    boost::asio::ip::tcp::socket& socket,
                                                    message_context ctx,
                                                    uint32_t messageSize,
                                                    const boost::system::error_code& error);
                
                netutil();
                ~netutil();
            };
            
            ///
            /// Context for read_message
            ///
            struct message_context
            {
                netutil::message_callback callback;
                sopmq::messages::message_type type;
                uint32_t message_size;
                char* message_buffer;
            };
            
        }
    }
}

#endif /* defined(__Project__netutil__) */
