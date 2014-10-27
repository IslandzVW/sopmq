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

#ifndef __sopmq__csunauthenticated__
#define __sopmq__csunauthenticated__

#include "iconnection_state.h"
#include "connection_in.h"
#include "message_dispatcher.h"
#include "message_ptrs.h"
#include "network_operation_result.h"
#include "ring.h"

#include "GetChallengeMessage.pb.h"

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#include <cstdint>
#include <memory>
#include <string>

namespace sopmq {
    namespace node {
        namespace connection {
            
            ///
            /// State before a connection has been authenticated
            ///
            class csunauthenticated :   public iconnection_state,
                                        public boost::noncopyable,
                                        public std::enable_shared_from_this<csunauthenticated>
            {
            public:
                csunauthenticated(boost::asio::io_service& ioService, connection_in::ptr conn,
                    const ring& ring);
                virtual ~csunauthenticated();
                

                // iconnection_state
                virtual void start();
                virtual std::string get_description() const;
                // iconnection_state
                
            private:
                ///
                /// Size of the auth challenge sent to connectors
                ///
                static const int CHALLENGE_SIZE;
                
                
                boost::asio::io_service& _ioService;
                connection_in::ptr _conn;
                sopmq::message::message_dispatcher _dispatcher;
                const ring& _ring;
                GetChallengeMessage_Type _authType;
                
                std::string _challenge;
                
                bool _closeAfterTransmission;
                
                void unhandled_message(Message_ptr message);
                
                void handle_get_challenge_message(const shared::net::network_operation_result&, GetChallengeMessage_ptr message);
                
                void handle_read_result(const shared::net::network_operation_result& result);
                
                void generate_challenge_response(connection_in::ptr conn, std::uint32_t replyTo);
                
                void handle_answer_challenge_message(const shared::net::network_operation_result&, AnswerChallengeMessage_ptr message);
                
                void handle_write_result(const shared::net::network_operation_result& result);
                
            };
        }
    }
}

#endif /* defined(__sopmq__csunauthenticated__) */
