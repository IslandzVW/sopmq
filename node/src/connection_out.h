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

#ifndef __sopmq__connection_out__
#define __sopmq__connection_out__

#include <boost/noncopyable.hpp>
#include <memory>

#include "connection_base.h"

namespace sopmq {
    namespace error {
        class network_error;
    }
}

namespace sopmq {
    namespace node {
        
        class server;
        
        namespace connection {
            
            ///
            /// A connection to another SOPMQ node
            ///
            class connection_out :  public boost::noncopyable,
                                    public sopmq::shared::net::connection_base,
                                    public std::enable_shared_from_this<connection_out>
            {
                
            };
        }
    }
}

#endif /* defined(__sopmq__connection_out__) */
