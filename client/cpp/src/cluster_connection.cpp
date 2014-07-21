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
using sopmq::message::message_dispatcher;

namespace sopmq {
    namespace client {
        
        cluster_connection::cluster_connection(cluster_endpoint::ptr ep,
                                               ba::io_service& ioService)
        : _endpoint(ep), _ioService(ioService), _resolver(ioService),
        _query(ep->network_endpoint().host_name(), ""),
        _socket(ioService)
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
                                               ba::ip::tcp::resolver::iterator endpoint_iterator,
                                               connect_callback ccb)
        {
            if (!err)
            {
                //we have an endpoint, let's try a connect
                _socket.async_connect(*endpoint_iterator,
                                      std::bind(&cluster_connection::after_connect,
                                                this, _1, ccb));
            }
            else
            {
                //resolution failed
                ccb(false, err);
            }
        }
        
        void cluster_connection::after_connect(const boost::system::error_code& err,
                                               connect_callback ccb)
        {
            if (!err)
            {
                //connection is good, tell our callback
                ccb(true, err);
            }
            else
            {
                //connection failed
                ccb(false, err);
            }
        }
        
        void cluster_connection::send_message(Message_ptr message)
        {
            
        }
        
        void cluster_connection::set_dispatcher(sopmq::message::message_dispatcher* dispatcher)
        {
            _dispatcher = dispatcher;
        }
        
        void cluster_connection::close()
        {
            _socket.shutdown(ba::socket_base::shutdown_type::shutdown_both);
            _socket.close();
        }
        
        const shared::net::endpoint& cluster_connection::network_endpoint() const
        {
            return _endpoint->network_endpoint();
        }
        
    }
}