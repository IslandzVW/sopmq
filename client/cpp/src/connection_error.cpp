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

#include "connection_error.h"

namespace sopmq {
    namespace error {
        
        connection_error::connection_error(const std::string& what)
        : std::runtime_error(what)
        {
            
        }
        
        connection_error::connection_error(const boost::system::error_code& error)
        : std::runtime_error(error.message()), _network_error(error)
        {
            
        }
        
        connection_error::~connection_error()
        {
            
        }
        
        bool connection_error::was_network_error() const
        {
            return _network_error.value() != 0;
        }
        
        const boost::system::error_code& connection_error::get_network_error() const
        {
            return _network_error;
        }
        
    }
}