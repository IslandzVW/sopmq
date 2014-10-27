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

#ifndef __sopmq__csauthenticated__
#define __sopmq__csauthenticated__

#include "iconnection_state.h"

#include "connection_in.h"
#include "message_dispatcher.h"
#include "network_operation_result.h"
#include "message_ptrs.h"
#include "ring.h"

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>

namespace sopmq {
    namespace node {
        namespace connection {
            
            ///
            /// An authenticated connection from either a client or another node
            ///
            class csauthenticated : public iconnection_state,
                                    public boost::noncopyable,
                                    public std::enable_shared_from_this<csauthenticated>
            {
            public:
                csauthenticated(boost::asio::io_service& ioService, connection_in::ptr conn,
                    const ring& ring);
                virtual ~csauthenticated();
                
                //iconnection_state
                void start();
                std::string get_description() const;
                //
            
            private:
                boost::asio::io_service& _ioService;
                connection_in::ptr _conn;
                const ring& _ring;
                sopmq::message::message_dispatcher _dispatcher;
                
                
                void unhandled_message(Message_ptr message);
                
                void handle_read_result(const shared::net::network_operation_result& result);
                void handle_write_result(const shared::net::network_operation_result& result);
                
                ///
                /// Called when the client is requesting to post a message to the ring.
                ///
                void handle_post_message(const shared::net::network_operation_result& result, PublishMessage_ptr message);
            };
            
        }
    }
}

#endif /* defined(__sopmq__csauthenticated__) */
