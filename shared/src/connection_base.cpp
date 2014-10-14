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

#include "connection_base.h"



using namespace sopmq::message;

namespace sopmq {
    namespace shared {
        namespace net {
            
            connection_base::connection_base(boost::asio::io_service& ioService,
                                             const shared::net::endpoint& ep,
                                             uint32_t maxMessageSize)
            : _ioService(ioService), _endpoint(ep), _socket(ioService), _next_id(0),
            _max_message_size(maxMessageSize)
            {
                
            }
            
            connection_base::connection_base(boost::asio::io_service& ioService,
                                             uint32_t maxMessageSize)
            : _ioService(ioService), _endpoint(), _socket(ioService), _next_id(0),
            _max_message_size(maxMessageSize)
            {
                
            }
            
            connection_base::~connection_base()
            {
                
            }

            boost::asio::ip::tcp::socket& connection_base::get_socket()
            {
                return _socket;
            }
            
            std::uint32_t connection_base::get_next_id()
            {
                return ++_next_id;
            }
            
            void connection_base::send_message(message_type type, Message_ptr message,
                                               network_status_callback statusCb)
            {
                messageutil::write_message(type, message, _ioService, _socket,
                                           statusCb);
            }
            
            void connection_base::read_message(sopmq::message::message_dispatcher& dispatcher,
                                               sopmq::message::network_status_callback callback)
            {
                messageutil::read_message(_ioService, _socket,
                                          callback,
                                          dispatcher,
                                          _max_message_size);
            }
            
            void connection_base::close()
            {
                //we really don't care about errors here
                boost::system::error_code ec;
                _socket.close(ec);
            }
            
            void connection_base::resolve_connected_endpoint()
            {
                _endpoint = sopmq::shared::net::endpoint(_socket.remote_endpoint());
            }
            
        }
    }
}