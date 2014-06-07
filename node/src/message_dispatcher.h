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

#ifndef __Project__message_dispatcher__
#define __Project__message_dispatcher__

#include "message_ptrs.h"
#include <functional>

namespace sopmq {
    namespace message {
        
        ///
        /// Handles dispatch of network messages to selected functions
        ///
        class message_dispatcher
        {
        private:
            std::function<void(Message_ptr)> _unhandledHandler;
            std::function<void(GetChallengeMessage_ptr)>  _getChallengeHandler;
            
        public:
            message_dispatcher(std::function<void(Message_ptr)> unhandledHandler);
            virtual ~message_dispatcher();
            
            ///
            /// Dispatches the GetChallenge message to the correct handler
            ///
            void dispatch(GetChallengeMessage_ptr getChallengeMessage);
            
        public:
            ///
            /// Sets the handler function for a GetChallenge message
            ///
            void set_handler(std::function<void(GetChallengeMessage_ptr)> handler);
            
        private:
            ///
            /// Template function to execute the given handler if it is available, or
            /// the unhandled handler if it is not
            ///
            template <typename handler, typename message>
            void do_dispatch(handler h, message m)
            {
                if (h)
                {
                    h(m);
                }
                else
                {
                    _unhandledHandler(std::static_pointer_cast<::google::protobuf::Message>(m));
                }
            }
            
        };
        
    }
}


#endif /* defined(__Project__message_dispatcher__) */
