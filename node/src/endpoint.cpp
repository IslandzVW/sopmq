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

#include "endpoint.h"


namespace sopmq {
    namespace node {
        namespace net {
            
            endpoint::endpoint(const std::string& uri)
            {
                
            }

            endpoint::~endpoint()
            {
                
            }
            
            const std::string& endpoint::host_name() const
            {
                return _hostname;
            }
            
            unsigned short endpoint::port() const
            {
                return _port;
            }
            
            endpoint_proto endpoint::protocol() const
            {
                return _proto;
            }
            
        }
    }
}