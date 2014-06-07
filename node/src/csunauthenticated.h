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

#ifndef __Project__csunauthenticated__
#define __Project__csunauthenticated__

#include "iconnection_state.h"
#include "connection.h"
#include "message_dispatcher.h"
#include "message_ptrs.h"
#include "network_error.h"

#include <boost/asio.hpp>

namespace sopmq {
    namespace node {
        namespace connection {
            
            ///
            /// State before a connection has been authenticated
            ///
            class csunauthenticated : public iconnection_state
            {
            public:
                csunauthenticated(boost::asio::io_service& ioService, connection::wptr conn);
                virtual ~csunauthenticated();
                

                // iconnection_state
                virtual void start();
                virtual std::string get_description() const;
                // iconnection_state
                
            private:
                boost::asio::io_service& _ioService;
                connection::wptr _conn;
                sopmq::message::message_dispatcher _dispatcher;
                
                void unhandled_message(Message_ptr message);
                
                void handle_get_challenge_message(GetChallengeMessage_ptr message);
                
                void handle_network_error(const sopmq::error::network_error& error);
            };
        }
    }
}

#endif /* defined(__Project__csunauthenticated__) */
