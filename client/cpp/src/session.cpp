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

#include "session.h"

#include "cluster.h"
#include "GetChallengeMessage.pb.h"
#include "logging.h"
#include "authentication_state.h"
#include "messageutil.h"

using sopmq::message::message_dispatcher;
using sopmq::message::messageutil;
using namespace std::placeholders;

using namespace sopmq::client::impl;

namespace sopmq {
    namespace client {
        
        session::session(cluster::wptr cluster, cluster_connection::ptr initialConnection)
        : _cluster(cluster), _connection(initialConnection), _valid(true)
        {
            
        }
        
        session::~session()
        {
            
        }
        
        void session::authenticate(const std::string& username, const std::string& password,
                                   authenticate_callback authCallback)
        {
            _session_state.reset(new authentication_state(_connection, *this, username, password, authCallback));
            
            _username = username;
            _password = password;
            
            //send a request to the server to get an auth challenge
            GetChallengeMessage_ptr gcm = std::make_shared<GetChallengeMessage>();
            gcm->set_type(GetChallengeMessage::CLIENT);
            gcm->set_allocated_identity(messageutil::build_id(_connection->get_next_id(), 0));
            
            _connection->send_message(message::MT_GET_CHALLENGE, gcm, std::bind(&session::on_network_status, this, _1, _2));
        }
        
        void session::on_network_status(bool success, const sopmq::error::network_error &error)
        {
            
        }
        
        void session::protocol_violation()
        {
            this->invalidate();
        }
        
        void session::invalidate()
        {
            _valid = false;
            _connection->close();
        }
        
        void session::auth_callback(bool authSuccess)
        {
            if (! authSuccess)
            {
                this->invalidate();
            }
            
            
        }
    }
}