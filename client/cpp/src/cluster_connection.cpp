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
#include "settings.h"

#include <boost/lexical_cast.hpp>

namespace ba = boost::asio;
using namespace std::placeholders;

using sopmq::message::message_dispatcher;
using sopmq::message::network_status_callback;
using sopmq::message::messageutil;
using sopmq::message::message_type;
using sopmq::shared::net::network_operation_result;
using sopmq::error::network_error;
using sopmq::shared::net::connection_base;

namespace sopmq {
    namespace client {
        
        cluster_connection::cluster_connection(cluster_endpoint::ptr ep,
                                               ba::io_service& ioService)
        : connection_base(ioService, ep->network_endpoint(), settings::instance().maxMessageSize)
        {
            
        }
        
        cluster_connection::~cluster_connection()
        {
            
        }
        
    }
}