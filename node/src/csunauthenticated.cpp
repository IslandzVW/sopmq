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
#include "user_account.h"
#include "csauthenticated.h"

#include "ChallengeResponseMessage.pb.h"
#include "AnswerChallengeMessage.pb.h"
#include "Identifier.pb.h"
#include "AuthAckMessage.pb.h"

#include <functional>

using namespace std::placeholders;
using sopmq::message::messageutil;
using sopmq::node::settings;
using sopmq::error::network_error;
using sopmq::shared::util;
using sopmq::node::user_account;

namespace ba = boost::asio;

namespace sopmq {
    namespace node {
        namespace connection {
            
            const int csunauthenticated::CHALLENGE_SIZE = 1024;
            
            csunauthenticated::csunauthenticated(ba::io_service& ioService, connection_in::wptr conn)
            : _ioService(ioService), _conn(conn),
            _dispatcher(std::bind(&csunauthenticated::unhandled_message, this, _1)),
            _closeAfterTransmission(false)
            {
            }
            
            csunauthenticated::~csunauthenticated()
            {
                LOG_SRC(debug) << "~csunauthenticated()";
            }
            
            void csunauthenticated::unhandled_message(Message_ptr message)
            {
                if (auto connptr = _conn.lock())
                {
                    connptr->handle_error(network_error("Unexpected message received during authentication"));
                }
            }
            
            void csunauthenticated::handle_get_challenge_message(const shared::net::network_operation_result&, GetChallengeMessage_ptr message)
            {
                if (!message) return;
                
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
            
            void csunauthenticated::generate_challenge_response(connection_in::ptr conn, std::uint32_t replyTo)
            {
                ChallengeResponseMessage_ptr response = messageutil::make_message<ChallengeResponseMessage>(conn->get_next_id(), replyTo);
                
                _challenge = util::random_bytes(CHALLENGE_SIZE);
                
                response->set_challenge(_challenge);
                
                // clear the challenge handler.
                _dispatcher.set_handler(std::function<void(const shared::net::network_operation_result&, GetChallengeMessage_ptr)>());
                
                // set the new handler for the client answer
                std::function<void(const shared::net::network_operation_result&, AnswerChallengeMessage_ptr)> func
                    = std::bind(&csunauthenticated::handle_answer_challenge_message, this, _1, _2);
                
                _dispatcher.set_handler(func, response->identity().id());
                
                conn->send_message(message::MT_CHALLENGE_RESPONSE, response,
                                   std::bind(&csunauthenticated::handle_write_result, shared_from_this(), _1));
                this->read_next_message(conn);
            }
            
            void csunauthenticated::handle_answer_challenge_message(const shared::net::network_operation_result&, AnswerChallengeMessage_ptr message)
            {
                if (!message) return; //error
                
                LOG_SRC(debug) << "handle_answer_challenge_message()";
                auto self(shared_from_this());

                std::function<void(bool)> authCallback = [=](bool authd) {
                    //remember this is coming back from the libuv stuff inside the cassandra
                    //driver, so we need to get back into our IO thread
                    
                    if (authd)
                    {
                        //user is good to go
                        _ioService.post([=] {
                            auto connptr = self->_conn.lock();
                            if (connptr == nullptr) return;
                            
                            AuthAckMessage_ptr response = messageutil::make_message<AuthAckMessage>(connptr->get_next_id(), message->identity().id());
                            response->set_authorized(true);
                            connptr->send_message(message::MT_AUTH_ACK, response, std::bind(&csunauthenticated::handle_write_result,
                                                                                            self, _1));
                            csauthenticated::ptr authstate = std::make_shared<csauthenticated>(_ioService, connptr);
                            connptr->change_state(authstate);
                        });
                    }
                    else
                    {
                        //no good
                        _ioService.post([=] {
                            auto connptr = _conn.lock();
                            if (connptr == nullptr) return;
                            
                            _closeAfterTransmission = true;
                            AuthAckMessage_ptr response = messageutil::make_message<AuthAckMessage>(connptr->get_next_id(), message->identity().id());
                            response->set_authorized(false);
                            connptr->send_message(message::MT_AUTH_ACK, response, std::bind(&csunauthenticated::handle_write_result,
                                                                                            self, _1));
                        });
                    }
                };
                
                user_account::is_authorized(message->uname_hash(), _challenge, message->challenge_response(), authCallback);
            }
            
            void csunauthenticated::handle_read_result(const shared::net::network_operation_result& result)
            {
                if (auto connptr = _conn.lock())
                {
                    if (! result.was_successful())
                    {
                        connptr->handle_error(result.get_error());
                    }
                }
            }
                                   
            void csunauthenticated::handle_write_result(const shared::net::network_operation_result& result)
            {
                auto connptr = _conn.lock();
                if (connptr)
                {
                    if (!result.was_successful())
                    {
                        connptr->handle_error(result.get_error());
                    }
                    
                    if (_closeAfterTransmission)
                    {
                        connptr->handle_error(sopmq::error::network_error("connection forced closed by authentication"));
                    }
                }
            }
            
            void csunauthenticated::start()
            {
                std::function<void(const shared::net::network_operation_result&, GetChallengeMessage_ptr)> func
                    = std::bind(&csunauthenticated::handle_get_challenge_message, this, _1, _2);
                
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
            
            void csunauthenticated::read_next_message(connection_in::ptr conn)
            {
                conn->read_message(_dispatcher, std::bind(&csunauthenticated::handle_read_result, shared_from_this(), _1));
            }
        }
    }
}