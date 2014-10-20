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

#ifndef sopmq_responses_h
#define sopmq_responses_h

namespace sopmq {
    namespace shared {
        namespace message {
            
            ///
            /// Response code to a client publish request
            ///
            enum PublishMessageResponse {
                ///
                /// The message was piped to a consumer
                ///
                PMR_MESSAGE_PIPED = 1,
                
                ///
                /// The message couldn't be piped and was stored in persistent storage
                ///
                PMR_MESSAGE_STORED = 2,
                
                ///
                /// The message was dropped due to a timeout
                ///
                PMR_MESSAGE_DROPPED = 3,
                
                ///
                /// The connection was not authorized to make this request
                ///
                PMR_NOT_AUTH = 4,
                
                ///
                /// The nodes that handle this range were not available
                ///
                PMR_UNAVAILABLE = 5
            };
            
        }
    }
}

#endif
