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

#include "csunauthenticated.h"

#include <functional>

#include "messageutil.h"
#include "settings.h"

using namespace std::placeholders;
using sopmq::message::messageutil;
using sopmq::node::settings;
using sopmq::error::network_error;

namespace ba = boost::asio;

namespace sopmq {
    namespace node {
        namespace connection {
            
            csunauthenticated::csunauthenticated(ba::io_service& ioService, connection::wptr conn)
            : _ioService(ioService), _conn(conn), _dispatcher(std::bind(&csunauthenticated::unhandled_message, this, _1))
            {
				std::function<void(GetChallengeMessage_ptr)> func = std::bind(&csunauthenticated::handle_get_challenge_message, this, _1);
                _dispatcher.set_handler(func);
            }
            
            csunauthenticated::~csunauthenticated()
            {
                
            }
            
            void csunauthenticated::unhandled_message(Message_ptr message)
            {
                if (auto connptr = _conn.lock())
                {
                    connptr->handle_error(network_error("Unexpected message received during authentication"));
                }
            }
            
            void csunauthenticated::handle_get_challenge_message(GetChallengeMessage_ptr message)
            {
                
            }
            
            void csunauthenticated::handle_network_result(const net::network_operation_result& result)
            {
                if (auto connptr = _conn.lock())
                {
                    if (! result.was_successful())
                    {
                        connptr->handle_error(result.get_error());
                    }
                }
            }
            
            void csunauthenticated::start()
            {
                if (auto connptr = _conn.lock())
                {
                    //read a message from the network
                    messageutil::read_message(_ioService, connptr->get_socket(),
                                              std::bind(&csunauthenticated::handle_network_result, this, _1),
                                              _dispatcher, settings::instance().maxMessageSize);
                }
            }
            
            std::string csunauthenticated::get_description() const
            {
                return "unauthenticated";
            }
            
        }
    }
}