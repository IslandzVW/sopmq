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

#include "netutil.h"

#include <boost/pool/pool.hpp>
#include <boost/shared_array.hpp>
#include <boost/asio/detail/socket_ops.hpp>

using namespace std::placeholders;

namespace sopmq {
    namespace node {
        namespace util {
            
            boost::pool<> netutil::s_mem_pool(sizeof(uint32_t));
            
            netutil::netutil()
            {
                
            }
            
            netutil::~netutil()
            {
                
            }
            
            void netutil::read_u32(boost::asio::io_service& ioService,
                                   boost::asio::ip::tcp::socket& socket,
                                   std::function<void(uint32_t, const boost::system::error_code& error)> callback)
            {
                char* buffer = (char*)s_mem_pool.malloc();
                boost::asio::async_read(socket,
                                        boost::asio::buffer(buffer, 4),
                                        std::bind(&netutil::after_u32_read,
                                                    buffer,
                                                    callback,
                                                    _1,
                                                    _2));
            }
            
            
            
            void netutil::read_u16(boost::asio::io_service& ioService,
                                   boost::asio::ip::tcp::socket& socket,
                                   std::function<void(uint16_t, const boost::system::error_code& error)> callback)
            {
                char* buffer = (char*)s_mem_pool.malloc();
                boost::asio::async_read(socket,
                                        boost::asio::buffer(buffer, 2),
                                        std::bind(&netutil::after_u16_read,
                                                  buffer,
                                                  callback,
                                                  _1,
                                                  _2));
            }
            
            void netutil::after_u32_read(char* buffer,
                                         std::function<void(uint32_t, const boost::system::error_code& error)> callback,
                                         const boost::system::error_code& error,
                                         std::size_t bytes_transferred)
            {
                boost::shared_array<char> bufferptr(buffer, &netutil::free_mem);
                
                if (error)
                {
                    callback(0, error);
                    return;
                }
                
                uint32_t hl = boost::asio::detail::socket_ops::network_to_host_long(*(uint32_t*)buffer);
                callback(hl, error);
            }
            
            void netutil::after_u16_read(char* buffer,
                                         std::function<void(uint16_t, const boost::system::error_code& error)> callback,
                                         const boost::system::error_code& error,
                                         std::size_t bytes_transferred)
            {
                boost::shared_array<char> bufferptr(buffer, &netutil::free_mem);
                
                if (error)
                {
                    callback(0, error);
                    return;
                }
                
                uint16_t hl = boost::asio::detail::socket_ops::network_to_host_short(*(uint16_t*)buffer);
                callback(hl, error);
            }
            
            void netutil::free_mem(char* mem)
            {
                s_mem_pool.free(mem);
            }
            
            void netutil::read_message(boost::asio::io_service& ioService,
                                       boost::asio::ip::tcp::socket& socket,
                                       message_callback callback)
            {
                //read the message type
                read_u16(ioService, socket, std::bind(&after_read_message_type,
                                                      std::ref(ioService), std::ref(socket),
                                                      callback,
                                                      _1, _2));
            }
            
            void netutil::after_read_message_type(boost::asio::io_service& ioService,
                                                  boost::asio::ip::tcp::socket& socket,
                                                  message_callback callback,
                                                  uint16_t messageType,
                                                  const boost::system::error_code& error)
            {
                if (error)
                {
                    callback(nullptr, sopmq::messages::MT_INVALID, error);
                    return;
                }
                
                
            }
            
        }
    }
}