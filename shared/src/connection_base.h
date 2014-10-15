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

#ifndef __sopmq__connection_base__
#define __sopmq__connection_base__

#include "endpoint.h"
#include "message_types.h"
#include "message_ptrs.h"
#include "messageutil.h"
#include "message_dispatcher.h"

#include <boost/asio.hpp>

#include <cstdint>
#include <memory>

namespace sopmq {
    namespace shared {
        namespace net {
            
            ///
            /// Encapsulates basic functionality that is needed for any connection
            ///
            class connection_base
            {
            public:
                ///
                /// Constructor for an outbound connection
                ///
                connection_base(boost::asio::io_service& ioService,
                                const shared::net::endpoint& ep,
                                uint32_t maxMessageSize);
                
                ///
                /// Constructor for an inbound connection
                ///
                connection_base(boost::asio::io_service& ioService,
                                uint32_t maxMessageSize);
                
                virtual ~connection_base();
                
                ///
                /// Returns the TCP socket associated with this connection
                ///
                boost::asio::ip::tcp::socket& get_socket();
                
                ///
                /// Returns the next message identifier on this connection
                ///
                std::uint32_t get_next_id();
                
                ///
                /// Returns the endpoint this socket is connected to
                ///
                const shared::net::endpoint& endpoint() const;
                
                ///
                /// Resolves our endpoint from the connected socket
                ///
                void resolve_connected_endpoint();
                
                ///
                /// Begins an async connect to the enpoint provided in the ctor
                ///
                void connect(sopmq::message::network_status_callback ccb);
                
                ///
                /// Sends a message over this connection
                ///
                void send_message(sopmq::message::message_type type, Message_ptr message,
                                  sopmq::message::network_status_callback statusCb);
                
                ///
                /// Reads a message from this connection
                ///
                void read_message(sopmq::message::message_dispatcher& dispatcher,
                                  sopmq::message::network_status_callback callback);
                
                ///
                /// Closes the connection
                ///
                void close();
                
            private:
                void after_resolve(const boost::system::error_code& err,
                                   boost::asio::ip::tcp::resolver::iterator endpoint_iterator,
                                   message::network_status_callback ccb,
                                   std::shared_ptr<boost::asio::ip::tcp::resolver> resolver,
                                   std::shared_ptr<boost::asio::ip::tcp::resolver::query> query);
                
                void after_connect(const boost::system::error_code& err,
                                   message::network_status_callback ccb);
                
                boost::asio::io_service& _ioService;
                shared::net::endpoint _endpoint;
                boost::asio::ip::tcp::socket _socket;
                std::uint32_t _next_id;
                uint32_t _max_message_size;
            };
            
        }
    }
}

#endif /* defined(__sopmq__connection_base__) */
