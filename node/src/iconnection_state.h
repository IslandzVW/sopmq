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

#ifndef Project_iconnection_state_h
#define Project_iconnection_state_h

#include <string>
#include <memory>

namespace sopmq {
    namespace node {
        namespace connection {
            
            ///
            /// The state that a connection is currently in
            ///
            class iconnection_state
            {
            public:
                typedef std::shared_ptr<iconnection_state> ptr;
                typedef std::weak_ptr<iconnection_state> wptr;
                
            public:
                iconnection_state();
                virtual ~iconnection_state();
                
                ///
                /// Begins this new state
                ///
                virtual void start() = 0;
                
                ///
                /// Returns a description of this state
                ///
                virtual std::string get_description() const = 0;
            };
        }
    }
}

#endif
