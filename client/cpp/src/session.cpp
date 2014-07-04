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

namespace sopmq {
    namespace client {
        
        session::session(cluster::wptr cluster, cluster_connection::ptr initialConnection)
        : _cluster(cluster), _connection(initialConnection), _nextId(0)
        {
            
        }
        
        session::~session()
        {
            
        }
        
        void session::authenticate(const std::string& username, const std::string& password,
                                   authenticate_callback authCallback)
        {
            //send a request to the server to get an auth challenge
            GetChallengeMessage gcm;
            gcm.set_type(GetChallengeMessage::CLIENT);
            gcm.set_id(++_nextId);
            
            _connection->send_message(gcm);
        }
        
    }
}