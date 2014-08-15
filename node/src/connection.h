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

#ifndef __sopmq__connection__
#define __sopmq__connection__

#include "iconnection_state.h"

#include "message_ptrs.h"
#include "message_types.h"
#include "messageutil.h"

#include <boost/weak_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include <memory>
#include <cstdint>

namespace sopmq {
    namespace error {
        class network_error;
    }
}

namespace sopmq {
    namespace node {
        
        class server;
        
        namespace connection {
        
            ///
            /// Connections from clients or other servers to this server
            ///
            class connection :  public boost::noncopyable,
                                public std::enable_shared_from_this<connection>
            {
            public:
                typedef std::shared_ptr<connection> ptr;
                typedef std::weak_ptr<connection> wptr;
                
            public:
                connection(boost::asio::io_service& ioService);
                virtual ~connection();
                
                ///
                /// Returns the TCP socket associated with this connection
                ///
                boost::asio::ip::tcp::socket& get_socket();
                
                ///
                /// Starts this connection and informs our server we're alive
                ///
                void start(server* server);
                
                ///
                /// Handles an error from a child state. Disconnects the user
                ///
                void handle_error(const sopmq::error::network_error& e);
                
                ///
                /// Closes this connection and tells our server it has been closed
                ///
                void close();
                
                ///
                /// Returns the next message identifier on this connection
                ///
                std::uint32_t get_next_id();
                
                ///
                /// Sends a message over this connection
                ///
                void send_message(sopmq::message::message_type type, Message_ptr message,
                                  sopmq::message::network_status_callback statusCb);
                
            private:
                boost::asio::io_service& _ioService;
                boost::asio::ip::tcp::socket _conn;
                server* _server;
                boost::asio::ip::tcp::endpoint _ep;
                iconnection_state::ptr _state;
                
                std::uint32_t _next_id;
            };
        
        
        }
    }
}

#endif /* defined(__sopmq__connection__) */
