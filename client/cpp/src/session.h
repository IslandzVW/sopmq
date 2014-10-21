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

#ifndef __sopmq__session__
#define __sopmq__session__

#include "cluster_connection.h"
#include "message_dispatcher.h"
#include "message_ptrs.h"
#include "isession_state.h"
#include "responses.h"

#include <boost/noncopyable.hpp>
#include <memory>
#include <string>
#include <cstdint>
#include <functional>

namespace sopmq {
    namespace client {
        
        class cluster;
        
        
        ///
        /// A session with a sopmq cluster.
        ///
        class session : public boost::noncopyable, public std::enable_shared_from_this<session>
        {
        public:
            typedef std::shared_ptr<session> ptr;
            typedef std::weak_ptr<session> wptr;
            
            typedef std::function<void(bool)> authenticate_callback;
            typedef std::function<void(sopmq::shared::message::PublishMessageResponse)> publish_message_callback;
            
        
        public:
            ///
            /// Constructs a new session with the given initial connection
            ///
            session(std::weak_ptr<cluster> cluster,
                    cluster_connection::ptr initialConnection);
            virtual ~session();
            
            ///
            /// Authenticates to the cluster as a client
            ///
            void authenticate(const std::string& username, const std::string& password,
                              authenticate_callback authCallback);
            
            ///
            /// Posts a message to the given message queue
            /// \param queueId The queue to post the message to
            /// \param storeIfCantPipe Whether or not the message should be stored if it couldn't
            /// be piped to a consumer within its TTL
            /// \param ttl The time to live in seconds for this message
            /// \param data The message data
            /// \param callback The function that will be called after the publish operation completess
            void publish_message(const std::string& queueId, bool storeIfCantPipe, int ttl,
                                 const std::string& data, publish_message_callback callback);
            
            ///
            /// Indicates a protocol violation happened. Disconnects the connection
            ///
            void protocol_violation();
            
        private:
            void invalidate();
            void on_auth_status(bool authd);
            
            std::weak_ptr<cluster> _cluster;
            cluster_connection::ptr _connection;
            bool _valid;
            
            impl::isession_state::ptr _session_state;
            
            std::string _username;
            std::string _password;
        };
        
    }
}

#endif /* defined(__sopmq__session__) */
