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

#ifndef __sopmq__authentication_state__
#define __sopmq__authentication_state__

#include "message_dispatcher.h"
#include "cluster_connection.h"
#include "isession_state.h"
#include "network_error.h"
#include "network_operation_result.h"

#include <memory>

namespace sopmq {
    namespace client {
        
        class session; //fwd
        
        namespace impl {
            
            ///
            /// Client session state before the client has been authorized
            /// by a SOPMQ node
            ///
            class authentication_state : public isession_state, public std::enable_shared_from_this<authentication_state>
            {
            public:
                authentication_state(cluster_connection::ptr conn, std::weak_ptr<session> session,
                                     const std::string& username, const std::string& password,
                                     std::function<void(bool)> authCallback);
                virtual ~authentication_state();
                
                virtual void state_entry();
                
            private:
                void on_unhandled_message(Message_ptr message, const std::string& typeName);
                
                void on_challenge_response(ChallengeResponseMessage_ptr response);
                
                void on_auth_ack(AuthAckMessage_ptr response);
                
                void on_message_sent(const net::network_operation_result& result);
                
                void on_message_received(const net::network_operation_result& result);
                
                cluster_connection::ptr _connection;
                std::weak_ptr<session> _session;
                std::string _username;
                std::string _password;
                std::function<void(bool)> _authCallback;
                sopmq::message::message_dispatcher::ptr _dispatcher;
            };
            
        }
    }
}


#endif /* defined(__sopmq__authentication_state__) */
