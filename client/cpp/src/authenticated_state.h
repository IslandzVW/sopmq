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

#ifndef __sopmq__authenticated_state__
#define __sopmq__authenticated_state__

#include "isession_state.h"
#include "cluster_connection.h"
#include "message_dispatcher.h"
#include "message_ptrs.h"

#include <memory>

namespace sopmq {
    namespace client {
        
        class session; //fwd
        
        namespace impl {
            
            class authenticated_state : public isession_state,
                                        public std::enable_shared_from_this<authenticated_state>
            {
            public:
                authenticated_state(cluster_connection::ptr conn, std::weak_ptr<session> session);
                virtual ~authenticated_state();
                
                virtual void state_entry();
                
                virtual void publish_message(const std::string& queueId, bool storeIfCantPipe, int ttl,
                                             const std::string& data, publish_message_callback callback);
                
            private:
                void on_unhandled_message(Message_ptr message, const std::string& typeName);
                void read_next();
                
                cluster_connection::ptr _conn;
                std::weak_ptr<session> _session;
                
                sopmq::message::message_dispatcher _dispatcher;
            };
            
        }
    }
}


#endif /* defined(__sopmq__authenticated_state__) */
