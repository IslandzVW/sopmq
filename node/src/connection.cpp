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

#include "connection.h"

#include "server.h"
#include "logging.h"
#include "csunauthenticated.h"
#include "server.h"

#include <string>

namespace ba = boost::asio;
using sopmq::error::network_error;

namespace sopmq {
    namespace node {
        namespace connection {
            
            connection::connection(ba::io_service& ioService)
            : _ioService(ioService), _conn(_ioService)
            {
                
            }
            
            ba::ip::tcp::socket& connection::get_socket()
            {
                return _conn;
            }
            
            void connection::start(server* server)
            {
                _server = server;
                _server->connection_started(shared_from_this());
                
                try {
                    _ep = _conn.remote_endpoint();
                    LOG_SRC(debug) << "new connection from " << _ep.address().to_string();
                    
                } catch (const boost::system::system_error& e) {
                    //remote_endpoint can throw if the socket is disconnected
                    throw network_error(std::string("connection startup error") + e.what());
                }
                
                _state.reset(new csunauthenticated(_ioService, shared_from_this()));
            }
        }
    }
}