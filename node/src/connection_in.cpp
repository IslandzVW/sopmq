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
#include "settings.h"

#include <string>

namespace ba = boost::asio;
using sopmq::error::network_error;
using sopmq::message::message_type;
using sopmq::message::network_status_callback;
using sopmq::message::messageutil;
using sopmq::shared::net::connection_base;
using sopmq::node::settings;
using sopmq::shared::net::endpoint;

namespace sopmq {
    namespace node {
        namespace connection {
            
            connection_in::connection_in(ba::io_service& ioService)
            : connection_base(ioService, settings::instance().maxMessageSize),
            _io_service(ioService)
            {
                
            }
            
            connection_in::~connection_in()
            {
            }
            
            void connection_in::start(server* server)
            {
                this->resolve_connected_endpoint();
                
                _server = server;
                _server->connection_started(shared_from_this());
                
                _state = std::make_shared<csunauthenticated>(_io_service, shared_from_this());
                _state->start();
            }
            
            void connection_in::handle_error(const network_error& e)
            {
                LOG_SRC(error) << "network error: " << e.what() << ". closing connection";
                
                this->close();
            }
            
            void connection_in::change_state(iconnection_state::ptr newState)
            {
                _state = newState;
                _state->start();
            }
        }
    }
}