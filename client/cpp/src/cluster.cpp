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

#include "cluster.h"
#include "endpoint.h"

#include <algorithm>

namespace ba = boost::asio;
using sopmq::shared::net::endpoint;

namespace sopmq {
    namespace client {
        
        cluster::~cluster()
        {
            
        }
        
        void cluster::shuffle_endpoints()
        {
            std::random_shuffle(_liveEndpoints.begin(), _liveEndpoints.end());
        }
        
        void cluster::connect(boost::asio::io_service &ioService, connect_handler handler)
        {
            //connect to whatever endpoint is now at the top of the vector
            ba::ip::tcp::resolver resolver(ioService);
        }
        
    }
}