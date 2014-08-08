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
#include "random_selector.h"

#include <algorithm>


namespace ba = boost::asio;

using namespace std::placeholders;
using sopmq::error::connection_error;
using sopmq::shared::random_selector;

namespace sopmq {
    namespace client {
        
        cluster::~cluster()
        {
            
        }
        
        void cluster::connect(boost::asio::io_service &ioService, connect_handler handler)
        {
            try_connect_next_endpoint(handler, ioService);
        }
        
        void cluster::try_connect_next_endpoint(connect_handler handler,
                                                boost::asio::io_service &ioService)
        {
            check_for_expired_deaths();
            
            //any more endpoints left?
            if (_liveEndpoints.size() == 0)
            {
                handler(nullptr, connection_error("no nodes available"));
                return;
            }
            
            cluster_endpoint::ptr ep = this->random_endpoint();
            cluster_connection::ptr conn = std::make_shared<cluster_connection>(ep, ioService);
            
            connect_context ctx =
                {
                    handler,
                    ep,
                    conn
                };
            
            conn->connect(std::bind(&cluster::connection_result, this, _1, ctx));
        }
        
        void cluster::connection_result(const net::network_operation_result& result,
                                        connect_context ctx)
        {
            if (result.was_successful())
            {
                
            }
            else
            {
                ctx.handler(nullptr, error::connection_error(result.get_error().what()));
            }
        }
        
        void cluster::kill_endpoint(cluster_endpoint::ptr ep)
        {
            if (_deadEndpoints.find(ep) == _deadEndpoints.end())
            {
                //endpoint is still marked up, kill it
                ep->mark_failed();
                _deadEndpoints.insert(ep);
                
                _liveEndpoints.erase(std::remove(_liveEndpoints.begin(), _liveEndpoints.end(), ep));
            }
        }
        
        void cluster::check_for_expired_deaths()
        {
            std::vector<cluster_endpoint::ptr> retryEps;
            for (cluster_endpoint::ptr ep : _deadEndpoints)
            {
                if (ep->ready_for_retry())
                {
                    ep->mark_up();
                    retryEps.push_back(ep);
                }
            }
            
            std::for_each(retryEps.begin(), retryEps.end(),
                          [&](cluster_endpoint::ptr ep)
            {
                _deadEndpoints.erase(ep);
                _liveEndpoints.push_back(ep);
            });
        }
        
        cluster_endpoint::ptr cluster::random_endpoint()
        {
            sopmq::shared::random_selector<> selector;
            
            return selector(_liveEndpoints);
        }
        
    }
}