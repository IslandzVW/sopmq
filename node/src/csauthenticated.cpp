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

#include "csauthenticated.h"

#include "messageutil.h"
#include "network_error.h"
#include "settings.h"
#include "logging.h"

#include <functional>

using namespace std::placeholders;

using sopmq::error::network_error;
using sopmq::message::messageutil;
using sopmq::node::settings;

namespace sopmq {
    namespace node {
        namespace connection {
            
            csauthenticated::csauthenticated(boost::asio::io_service& ioService, connection_in::wptr conn)
            : _ioService(ioService), _conn(conn),
            _dispatcher(std::bind(&csauthenticated::unhandled_message, this, _1))
            {
                LOG_SRC(debug) << "csauthenticated()";
            }
            
            csauthenticated::~csauthenticated()
            {
                
            }
            
            void csauthenticated::start()
            {
                //set us up to handle any messages that clients should be sending to us
                std::function<void(const shared::net::network_operation_result&,PublishMessage_ptr)> func
                    = std::bind(&csauthenticated::handle_post_message, this, _1, _2);
                
                _dispatcher.set_handler(func);
                
                if (auto connptr = _conn.lock())
                {
                    this->read_next_message(connptr);
                }
            }
            
            void csauthenticated::unhandled_message(Message_ptr message)
            {
                if (auto connptr = _conn.lock())
                {
                    connptr->handle_error(network_error("Unexpected message received during operation"));
                }
            }
            
            void csauthenticated::read_next_message(connection_in::ptr conn)
            {
                conn->read_message(_dispatcher, std::bind(&csauthenticated::handle_read_result, shared_from_this(), _1));
            }
            
            void csauthenticated::handle_read_result(const shared::net::network_operation_result& result)
            {
                
            }
            
            void csauthenticated::handle_post_message(const shared::net::network_operation_result& result, PublishMessage_ptr message)
            {
                
            }
            
            std::string csauthenticated::get_description() const
            {
                return "csauthenticated";
            }
        }
    }
}