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

#ifndef __sopmq__connection_base__
#define __sopmq__connection_base__

#include "endpoint.h"

#include <boost/asio.hpp>

namespace sopmq {
    namespace shared {
        namespace net {
            
            ///
            /// Encapsulates basic functionality that is needed for any connection
            ///
            class connection_base
            {
            public:
                connection_base(boost::asio::io_service& ioService,
                                const shared::net::endpoint& ep);
                
                connection_base(boost::asio::io_service& ioService,
                                boost::asio::ip::tcp::socket socket);
                
                virtual ~connection_base();
                
            private:
                boost::asio::io_service& _ioService;
                shared::net::endpoint _endpoint;
                boost::asio::ip::tcp::resolver _resolver;
                boost::asio::ip::tcp::resolver::query _query;
                boost::asio::ip::tcp::socket _socket;
                
                
            };
            
        }
    }
}

#endif /* defined(__sopmq__connection_base__) */
