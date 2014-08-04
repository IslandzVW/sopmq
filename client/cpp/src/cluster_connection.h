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

#ifndef __Project__cluster_connection__
#define __Project__cluster_connection__

#include "cluster_endpoint.h"
#include "message_dispatcher.h"
#include "endpoint.h"
#include "message_ptrs.h"
#include "message_types.h"
#include "messageutil.h"

#include <boost/asio.hpp>
#include <functional>
#include <memory>
#include <cstdint>
#include <google/protobuf/message.h>

namespace sopmq {
    namespace client {
        
        ///
        /// An open connection to a coordinator node in the sopmq cluster
        ///
        class cluster_connection
        {
        public:
            typedef std::shared_ptr<cluster_connection> ptr;
            
            typedef std::function<void(bool, boost::system::error_code)> connect_callback;
            
        public:
            cluster_connection(cluster_endpoint::ptr ep,
                               boost::asio::io_service& ioService);
            virtual ~cluster_connection();
            
            ///
            /// Begins an async connect to the enpoint provided in the ctor
            ///
            void connect(connect_callback ccb);
            
            ///
            /// Sets the message dispatcher for this connection
            ///
            void set_dispatcher(sopmq::message::message_dispatcher* dispatcher);
            
            ///
            /// Sends a message through this connection
            ///
            void send_message(sopmq::message::message_type type, Message_ptr message,
                              sopmq::message::network_error_callback errorCb);
            
            ///
            /// Closes this connection. No further messages can be sent or received from it
            ///
            void close();
            
            ///
            /// Returns the endpoint that this connection is established with
            ///
            const shared::net::endpoint& network_endpoint() const;
            
            ///
            /// Returns the next message identifier for this connection
            ///
            std::uint32_t get_next_id();
            
            ///
            /// Listens for the next message from the wire and dispatches it to the assigned dispatcher
            ///
            void get_next_message(sopmq::message::network_error_callback errorCb);
            
        private:
            void after_resolve(const boost::system::error_code& err,
                               boost::asio::ip::tcp::resolver::iterator endpoint_iterator, connect_callback ccb);
            
            void after_connect(const boost::system::error_code& err,
                               connect_callback ccb);
            
            cluster_endpoint::ptr _endpoint;
            boost::asio::io_service& _ioService;
            boost::asio::ip::tcp::resolver _resolver;
            boost::asio::ip::tcp::resolver::query _query;
            boost::asio::ip::tcp::socket _socket;
            
            sopmq::message::message_dispatcher* _dispatcher;
            
            std::uint32_t _next_id;
        };
        
    }
}


#endif /* defined(__Project__cluster_connection__) */
