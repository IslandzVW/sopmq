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

#ifndef __sopmq__server__
#define __sopmq__server__

#include "connection.h"

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <set>

namespace sopmq {
    namespace node {
        
        ///
        /// Server that listens for and manages connections
        ///
        class server : public boost::noncopyable
        {
        public:
            server(boost::asio::io_service& ioService, unsigned short port);
            
            ///
            /// Starts this server to accept new connections
            ///
            void start();
            
            ///
            /// Called when a connection has been started and is alive
            ///
            void connection_started(connection::connection::ptr conn);
            
            ///
            /// Called when a connection has been terminated either locally or remotely
            ///
            void connection_terminated(connection::connection::ptr conn);
            
        private:
            boost::asio::io_service& _ioService;
            unsigned short _port;
            boost::asio::ip::tcp::endpoint _endpoint;
            boost::asio::ip::tcp::acceptor _acceptor;
            std::set<connection::connection::ptr> _connections;
            
            
            void accept_new();
            void handle_accept(connection::connection::ptr conn, const boost::system::error_code& error);
        };
        
    }
}


#endif /* defined(__sopmq__server__) */
