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
            
            csunauthenticated::csunauthenticated(ba::io_service& ioService, connection_in::ptr conn,
                const ring& ring)
            : _ioService(ioService), _conn(conn), _ring(ring),
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
                _conn->handle_error(network_error("Unexpected message received during authentication"));
            }
            
            void csunauthenticated::handle_get_challenge_message(const shared::net::network_operation_result&, GetChallengeMessage_ptr message)
            {
                if (!message) return;
                
                LOG_SRC(debug) << "handle_get_challenge_message()";
                
                _authType = message->type();
                this->generate_challenge_response(_conn, message->identity().id());
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
                _conn->read_message(_dispatcher, std::bind(&csunauthenticated::handle_read_result, shared_from_this(), _1));
            }

            void csunauthenticated::successful_auth(AnswerChallengeMessage_ptr message)
            {
                AuthAckMessage_ptr response = messageutil::make_message<AuthAckMessage>(_conn->get_next_id(), message->identity().id());
                response->set_authorized(true);
                _conn->send_message(message::MT_AUTH_ACK, response, std::bind(&csunauthenticated::handle_write_result,
                                                                              shared_from_this(), _1));
                csauthenticated::ptr authstate = std::make_shared<csauthenticated>(_ioService, _conn, _ring);
                _conn->change_state(authstate);
            }

            void csunauthenticated::failed_auth(AnswerChallengeMessage_ptr message)
            {
                _closeAfterTransmission = true;
                AuthAckMessage_ptr response = messageutil::make_message<AuthAckMessage>(_conn->get_next_id(), message->identity().id());
                response->set_authorized(false);
                _conn->send_message(message::MT_AUTH_ACK, response, std::bind(&csunauthenticated::handle_write_result,
                                                                              shared_from_this(), _1));
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
                            self->successful_auth(message);
                        });
                    }
                    else
                    {
                        //no good
                        _ioService.post([=] {
                            self->failed_auth(message);
                        });
                    }
                };
                
                //short circuit for unit tests
                if (_authType == GetChallengeMessage_Type_CLIENT && !settings::instance().unitTestUsername.empty())
                {
                    //check that the user trying to log in is using the unittest username
                    if (message->uname_hash() == util::sha256_hex_string(settings::instance().unitTestUsername))
                    {
                        self->successful_auth(message);
                        return;
                    }
                }
                
                if (_authType == GetChallengeMessage_Type_CLIENT)
                {
                    user_account::is_authorized(message->uname_hash(), _challenge, message->challenge_response(), authCallback);
                }
                else
                {
                    if (message->challenge_response() == util::sha256_hex_string(settings::instance().ring_key_hash() + _challenge))
                    {
                        self->successful_auth(message);
                    }
                    else
                    {
                        self->failed_auth(message);
                    }
                }
            }
            
            void csunauthenticated::handle_read_result(const shared::net::network_operation_result& result)
            {
                if (! result.was_successful())
                {
                    _conn->handle_error(result.get_error());
                }
            }
                                   
            void csunauthenticated::handle_write_result(const shared::net::network_operation_result& result)
            {
                if (!result.was_successful())
                {
                    _conn->handle_error(result.get_error());
                }
                    
                if (_closeAfterTransmission)
                {
                    _conn->handle_error(sopmq::error::network_error("connection forced closed by authentication"));
                }
            }
            
            void csunauthenticated::start()
            {
                std::function<void(const shared::net::network_operation_result&, GetChallengeMessage_ptr)> func
                        = std::bind(&csunauthenticated::handle_get_challenge_message, this, _1, _2);
                
                _dispatcher.set_handler(func);

                _conn->read_message(_dispatcher, std::bind(&csunauthenticated::handle_read_result, shared_from_this(), _1));
            }
            
            std::string csunauthenticated::get_description() const
            {
                return "unauthenticated";
            }
        }
    }
}