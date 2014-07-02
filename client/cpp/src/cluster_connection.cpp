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

#include "cluster_connection.h"

namespace ba = boost::asio;
using namespace std::placeholders;

namespace sopmq {
    namespace client {
        
        cluster_connection::cluster_connection(cluster_endpoint::ptr ep,
                                               ba::io_service& ioService)
        : _endpoint(ep), _ioService(ioService), _resolver(ioService),
        _query(ep->network_endpoint().host_name(), "")
        {
            
        }
        
        cluster_connection::~cluster_connection()
        {
            
        }
        
        void cluster_connection::connect(connect_callback ccb)
        {
            _resolver.async_resolve(_query,
                                    std::bind(&cluster_connection::after_resolve,
                                              this, _1, _2, ccb));
        }
        
        void cluster_connection::after_resolve(const boost::system::error_code& err,
                                               boost::asio::ip::tcp::resolver::iterator endpoint_iterator,
                                               connect_callback ccb)
        {
            if (!err)
            {
                //we have an endpoint, let's try a connect
                
            }
            else
            {
                //resolution failed
                ccb(false, err);
            }
        }
        
    }
}