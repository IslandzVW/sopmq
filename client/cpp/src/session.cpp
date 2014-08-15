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
#include "logging.h"

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
            this->invalidate();
            LOG_SRC(debug) << "~session()";
        }
        
        void session::authenticate(const std::string& username, const std::string& password,
                                   authenticate_callback authCallback)
        {
            _username = username;
            _password = password;
            
            _session_state = std::make_shared<authentication_state>(_connection, shared_from_this(),
                                                                    username, password, authCallback);
            _session_state->state_entry();
        }
        
        void session::protocol_violation()
        {
            LOG_SRC(error) << "protocol violation";
            this->invalidate();
        }
        
        void session::invalidate()
        {
            _valid = false;
            _connection->close();
        }
    }
}