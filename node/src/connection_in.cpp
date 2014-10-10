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

#include "connection_in.h"

#include "server.h"
#include "logging.h"
#include "csunauthenticated.h"
#include "server.h"

#include <string>

namespace ba = boost::asio;
using sopmq::error::network_error;
using sopmq::message::message_type;
using sopmq::message::network_status_callback;
using sopmq::message::messageutil;

namespace sopmq {
    namespace node {
        namespace connection {
            
            connection_in::connection_in(ba::io_service& ioService)
            : _ioService(ioService), _conn(_ioService), _next_id(0)
            {
                
            }
            
            connection_in::~connection_in()
            {
            }
            
            ba::ip::tcp::socket& connection_in::get_socket()
            {
                return _conn;
            }
            
            void connection_in::start(server* server)
            {
                _server = server;
                _server->connection_started(shared_from_this());
                
                try {
                    _ep = _conn.remote_endpoint();
                    LOG_SRC(debug) << "new connection from " << _ep.address().to_string();
                    
                } catch (const boost::system::system_error& e) {
                    //remote_endpoint() can throw if the socket is disconnected
                    throw network_error(std::string("connection startup error") + e.what());
                }
                
                _state = std::make_shared<csunauthenticated>(_ioService, shared_from_this());
                _state->start();
            }
            
            void connection_in::handle_error(const network_error& e)
            {
                LOG_SRC(error) << "network error: " << e.what() << ". closing connection";
                
                this->close();
            }
            
            void connection_in::close()
            {
                //we really don't care about errors here
                boost::system::error_code ec;
                _conn.close(ec);
                
                _server->connection_terminated(shared_from_this());
            }
            
            std::uint32_t connection_in::get_next_id()
            {
                return ++_next_id;
            }
            
            void connection_in::send_message(message_type type, Message_ptr message,
                                          network_status_callback statusCb)
            {
                messageutil::write_message(type, message, _ioService, _conn,
                                           statusCb);
            }
            
            void connection_in::change_state(iconnection_state::ptr newState)
            {
                _state = newState;
                _state->start();
            }
        }
    }
}