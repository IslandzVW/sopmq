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

#include "authentication_state.h"
#include "session.h"
#include "logging.h"
#include "ChallengeResponseMessage.pb.h"
#include "AnswerChallengeMessage.pb.h"

#include <functional>
#include <boost/assert.hpp>
#include <cryptopp/sha.h>

using sopmq::message::message_dispatcher;
using namespace std::placeholders;

namespace sopmq {
    namespace client {
        namespace impl {
            
            authentication_state::authentication_state(cluster_connection::ptr conn, session& session,
                                                       const std::string& username, const std::string& password)
            : _connection(conn), _session(session), _username(username), _password(password)
            {
                _dispatcher =
                    std::make_shared<message_dispatcher>(std::bind(&authentication_state::on_unhandled_message,
                                                                   this, _1, _2));
                
                _dispatcher->set_handler(std::bind(&authentication_state::on_challenge_response, this, _1));
                
                _connection->set_dispatcher(_dispatcher.get());
            }
            
            authentication_state::~authentication_state()
            {
                
            }
            
            void authentication_state::on_unhandled_message(Message_ptr message, const std::string& typeName)
            {
                LOG_SRC(error) << _connection->network_endpoint()
                    << " protocol violation: unexpected message: " << typeName;
                
                _session.protocol_violation();
            }
            
            void authentication_state::on_challenge_response(ChallengeResponseMessage_ptr response)
            {
                BOOST_ASSERT(response->has_challenge());
                
                const std::string& challenge = response->challenge();
                
                //respond to the challenge with:
                //sha256([username]) + " " + sha256(sha256([password])[challenge])
                unsigned char result[(CryptoPP::SHA256::DIGESTSIZE * 2) + 1];
                
                CryptoPP::SHA256().CalculateDigest(&result[0], (unsigned char*)_username.c_str(), _username.length());
                result[CryptoPP::SHA256::DIGESTSIZE] = ' ';
                CryptoPP::SHA256().CalculateDigest(&result[CryptoPP::SHA256::DIGESTSIZE + 1], (unsigned char*)_password.c_str(),
                                                   _password.length());
                
                AnswerChallengeMessage_ptr acm = std::make_shared<AnswerChallengeMessage>();
                acm->set_response(std::string((char*)result));
                _connection->send_message(acm);
            }
            
        }
    }
}