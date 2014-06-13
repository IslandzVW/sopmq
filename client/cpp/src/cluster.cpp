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
using namespace std::placeholders;

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
            if (_liveEndpoints.size() == 0)
            {
                handler(nullptr, sopmq::error::node_error_list(), "no endpoints available");
                return;
            }
            
            //connect to whatever endpoint is now at the top of the vector
            endpoint ep = _liveEndpoints[0];
            
            auto resolver = std::unique_ptr<ba::ip::tcp::resolver>(new ba::ip::tcp::resolver(ioService));
            
            auto query = std::unique_ptr<ba::ip::tcp::resolver::query>(new ba::ip::tcp::resolver::query(ep.host_name(), ""));
            
            resolver->async_resolve(*query,
                                    std::bind(&cluster::after_resolve, this,
                                              _1, _2));
        }
        
        void cluster::after_resolve(const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
        {
            
        }
        
    }
}