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

#include <boost/noncopyable.hpp>

namespace sopmq {
    namespace node {
        namespace connection {
            
            class csauthenticated : public iconnection_state,
                                    public boost::noncopyable,
                                    public std::enable_shared_from_this<csauthenticated>
            {
                
            };
            
        }
    }
}

#endif /* defined(__sopmq__csauthenticated__) */
