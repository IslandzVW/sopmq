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

#include "message_dispatcher.h"

#include "GetChallengeMessage.pb.h"

namespace sopmq {
    namespace message {
        
        message_dispatcher::message_dispatcher(std::function<void(Message_ptr)> unhandledHandler)
        : _unhandledHandler(unhandledHandler)
        {
            
        }
        
        message_dispatcher::~message_dispatcher()
        {
            
        }
        
        void message_dispatcher::dispatch(GetChallengeMessage_ptr getChallengeMessage)
        {
            do_dispatch(_getChallengeHandler, getChallengeMessage);
        }
        
        void message_dispatcher::set_handler(std::function<void(GetChallengeMessage_ptr)> handler)
        {
            _getChallengeHandler = handler;
        }
    }
}

