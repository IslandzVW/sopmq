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

#include "network_operation_result.h"

namespace sopmq {
    namespace net {
    
        network_operation_result::network_operation_result(error_type et,
                                                           const error::network_error& error)
        : _et(et), _error(error)
        {
            
        }
        
        network_operation_result::network_operation_result(error_type et)
        : _et(et), _error(error::network_error())
        {
            
        }
        
        network_operation_result::~network_operation_result()
        {
        
        }
        
        const network_operation_result& network_operation_result::success()
        {
            static network_operation_result successResult(ET_NONE,
                                                          error::network_error());
            
            return successResult;
        }
        
        network_operation_result network_operation_result::from_error_code(const boost::system::error_code &e)
        {
            return network_operation_result(ET_NETWORK, error::network_error(e));
        }
        
        network_operation_result network_operation_result::from_error_code(const std::string& info,
                                                                           const boost::system::error_code &e)
        {
            return network_operation_result(ET_NETWORK, error::network_error(info + ": " + e.message()));
        }
        
        bool network_operation_result::was_successful() const
        {
            return _et == error_type::ET_NONE;
        }
        
        error_type network_operation_result::get_error_type() const
        {
            return _et;
        }
        
        const error::network_error& network_operation_result::get_error() const
        {
            return _error;
        }
    }
}