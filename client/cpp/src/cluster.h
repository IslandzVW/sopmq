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
#include "connection_error.h"
#include "session.h"

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>

#include <memory>
#include <vector>

namespace sopmq {
    namespace client {
        
        ///
        /// A cluster is a collection of endpoints that is managed to provide
        /// fault tolerance in the face of errors
        ///
        class cluster : public boost::noncopyable
        {
        public:
            typedef std::shared_ptr<cluster> ptr;
            
            typedef std::function<void(session::ptr, sopmq::error::node_error_list)> connect_handler;
            
        public:
            template <typename ColType>
            cluster(ColType epCol)
            {
                for (auto ep : epCol)
                {
                    _liveEndpoints.push_back(ep);
                }
                
                shuffle_endpoints();
            }
            
            virtual ~cluster();
            
            ///
            /// Connects to the cluster and calls the handler on completion or error
            ///
            void connect(boost::asio::io_service& ioService, connect_handler handler);
            
        private:
            ///
            /// Shuffles the endpoints we have in our collection
            ///
            void shuffle_endpoints();
            
            ///
            /// Endpoints that we either know to be good, or haven't tried to connect to yet
            ///
            std::vector<shared::net::endpoint> _liveEndpoints;
            
            ///
            /// Endpoints that died on us
            ///
            std::vector<shared::net::endpoint> _deadEndpoints;
        };
        
    }
}