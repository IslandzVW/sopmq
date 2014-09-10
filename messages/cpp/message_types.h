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

#ifndef SHARED_MESSAGE_TYPES_H
#define SHARED_MESSAGE_TYPES_H

namespace sopmq {
    namespace message {
        
        ///
        /// SOPMQ message types
        ///
        enum message_type
        {
            MT_INVALID = 0,
            
            MT_GET_CHALLENGE,
            MT_CHALLENGE_RESPONSE,
            MT_ANSWER_CHALLENGE,
            MT_AUTH_ACK,
            MT_POST,
            MT_POST_RESPONSE,
            MT_SUBSCRIBE_QUEUE,
            MT_SUBSCRIBE_RESPONSE,
            
            MT_INVALID_OUT_OF_RANGE
        };
        
    }
}

#endif