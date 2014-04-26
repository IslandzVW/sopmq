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

#ifndef __Project__endpoint__
#define __Project__endpoint__

#include <string>

namespace sopmq {
    namespace node {
        namespace net {
            
            ///
            /// The endpoint protocol
            ///
            enum class endpoint_proto { SOPMQv1 };
            
            ///
            /// The endpoint for a node
            ///
            class endpoint
            {
            private:
                std::string _hostname;
                unsigned short _port;
                endpoint_proto _proto;
                
            public:
                ///
                /// Constructs an endpoint by parsing a uri
                ///
                endpoint(const std::string& uri);
                
                ///
                /// dtor
                ///
                virtual ~endpoint();

                
                ///
                /// The host name for this endpoint
                ///
                const std::string& host_name() const;
                
                ///
                /// The port for this endpoint
                ///
                unsigned short port() const;
                
                ///
                /// The protocol this endpoint speaks
                ///
                endpoint_proto protocol() const;
            };
            
        }
    }
}

#endif /* defined(__Project__endpoint__) */
