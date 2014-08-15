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

#include "messageutil.h"
#include "settings.h"
#include "logging.h"
#include "util.h"

#include "ChallengeResponseMessage.pb.h"
#include "AnswerChallengeMessage.pb.h"
#include "Identifier.pb.h"

#include <functional>

using namespace std::placeholders;
using sopmq::message::messageutil;
using sopmq::node::settings;
using sopmq::error::network_error;
using sopmq::shared::util;

namespace ba = boost::asio;

namespace sopmq {
    namespace node {
        namespace connection {
            
            const int csunauthenticated::CHALLENGE_SIZE = 1024;
            
            csunauthenticated::csunauthenticated(ba::io_service& ioService, connection::wptr conn)
            : _ioService(ioService), _conn(conn), _dispatcher(std::bind(&csunauthenticated::unhandled_message, this, _1))
            {
				
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
                LOG_SRC(debug) << "handle_get_challenge_message()";
                
                auto connptr = _conn.lock();
                if (connptr == nullptr) return;
                
                if (message->has_type())
                {
                    _authType = message->type();
                    
                    this->generate_challenge_response(connptr, message->identity().id());
                }
                else
                {
                    connptr->handle_error(network_error("GetChallengeMessage had no type specified"));
                }
            }
            
            void csunauthenticated::generate_challenge_response(connection::ptr conn, std::uint32_t replyTo)
            {
                ChallengeResponseMessage_ptr response = std::make_shared<ChallengeResponseMessage>();
                response->set_challenge(util::random_bytes(CHALLENGE_SIZE));
                response->set_allocated_identity(messageutil::build_id(conn->get_next_id(), replyTo));
                
                // clear the challenge handler.
                _dispatcher.set_handler(std::function<void(GetChallengeMessage_ptr)>());
                
                // set the new handler for the client answer
                std::function<void(AnswerChallengeMessage_ptr)> func
                    = std::bind(&csunauthenticated::handle_answer_challenge_message, shared_from_this(), _1);
                
                _dispatcher.set_handler(func);
                
                conn->send_message(message::MT_CHALLENGE_RESPONSE, response,
                                   std::bind(&csunauthenticated::handle_write_result, shared_from_this(), _1));
            }
            
            void csunauthenticated::handle_answer_challenge_message(AnswerChallengeMessage_ptr message)
            {
                LOG_SRC(debug) << "handle_answer_challenge_message()";
                
                
            }
            
            void csunauthenticated::handle_read_result(const net::network_operation_result& result)
            {
                if (auto connptr = _conn.lock())
                {
                    if (! result.was_successful())
                    {
                        connptr->handle_error(result.get_error());
                    }
                    else
                    {
                        this->read_next_message(connptr);
                    }
                }
            }
                                   
            void csunauthenticated::handle_write_result(const net::network_operation_result& result)
            {
                auto connptr = _conn.lock();
                if (!result.was_successful() && connptr)
                {
                    connptr->handle_error(result.get_error());
                }
            }
            
            void csunauthenticated::start()
            {
                std::function<void(GetChallengeMessage_ptr)> func
                    = std::bind(&csunauthenticated::handle_get_challenge_message, shared_from_this(), _1);
                
                _dispatcher.set_handler(func);
                
                if (auto connptr = _conn.lock())
                {
                    this->read_next_message(connptr);
                }
            }
            
            std::string csunauthenticated::get_description() const
            {
                return "unauthenticated";
            }
            
            void csunauthenticated::read_next_message(connection::ptr conn)
            {
                //read a message from the network
                messageutil::read_message(_ioService, conn->get_socket(),
                                          std::bind(&csunauthenticated::handle_read_result, this, _1),
                                          _dispatcher, settings::instance().maxMessageSize);
            }
        }
    }
}