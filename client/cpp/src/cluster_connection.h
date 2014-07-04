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

#ifndef __Project__cluster_connection__
#define __Project__cluster_connection__

#include "cluster_endpoint.h"

#include <boost/asio.hpp>
#include <functional>
#include <memory>
#include <google/protobuf/message.h>

namespace sopmq {
    namespace client {
        
        ///
        /// An open connection to a coordinator node in the sopmq cluster
        ///
        class cluster_connection
        {
        public:
            typedef std::shared_ptr<cluster_connection> ptr;
            
            typedef std::function<void(bool, boost::system::error_code)> connect_callback;
            
        public:
            cluster_connection(cluster_endpoint::ptr ep,
                               boost::asio::io_service& ioService);
            virtual ~cluster_connection();
            
            void connect(connect_callback ccb);
            
            void send_message(google::protobuf::Message& message);
            
            
        private:
            void after_resolve(const boost::system::error_code& err,
                               boost::asio::ip::tcp::resolver::iterator endpoint_iterator, connect_callback ccb);
            
            void after_connect(const boost::system::error_code& err,
                               connect_callback ccb);
            
            cluster_endpoint::ptr _endpoint;
            boost::asio::io_service& _ioService;
            boost::asio::ip::tcp::resolver _resolver;
            boost::asio::ip::tcp::resolver::query _query;
            boost::asio::ip::tcp::socket _socket;
        };
        
    }
}


#endif /* defined(__Project__cluster_connection__) */
