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

#ifndef __sopmq__cluster_connection__
#define __sopmq__cluster_connection__

#include "cluster_endpoint.h"
#include "message_dispatcher.h"
#include "endpoint.h"
#include "message_ptrs.h"
#include "message_types.h"
#include "messageutil.h"
#include "connection_base.h"

#include <boost/asio.hpp>
#include <functional>
#include <memory>
#include <cstdint>
#include <google/protobuf/message.h>

namespace sopmq {
    namespace client {
        
        ///
        /// An open connection to a coordinator node in the sopmq cluster
        ///
        class cluster_connection : public sopmq::shared::net::connection_base
        {
        public:
            typedef std::shared_ptr<cluster_connection> ptr;
            
        public:
            cluster_connection(cluster_endpoint::ptr ep,
                               boost::asio::io_service& ioService);
            virtual ~cluster_connection();
            
            
                         
        private:
        };
        
    }
}


#endif /* defined(__sopmq__cluster_connection__) */
