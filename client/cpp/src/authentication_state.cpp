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

#include <functional>
#include <boost/assert.hpp>

using sopmq::message::message_dispatcher;
using namespace std::placeholders;

namespace sopmq {
    namespace client {
        namespace impl {
            
            authentication_state::authentication_state(cluster_connection::ptr conn, session& session)
            : _connection(conn), _session(session)
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
                
                
            }
            
        }
    }
}