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

#ifndef __Project__connection_error__
#define __Project__connection_error__

#include "endpoint.h"

#include <boost/system/error_code.hpp>

#include <stdexcept>
#include <tuple>
#include <vector>


namespace sopmq {
    namespace error {
        
        class connection_error;
        
        ///
        /// A list of all the nodes we failed to connect to
        ///
        typedef std::vector<std::tuple<sopmq::shared::net::endpoint, connection_error>> node_error_list;
        
        ///
        /// Used to indicate an error occurred when connecting to a resource
        ///
        class connection_error : public std::runtime_error
        {
        public:
            connection_error(const std::string& what);
            connection_error(const boost::system::error_code& error);
            virtual ~connection_error();
            
            ///
            /// Does this exception contain errors from one or more nodes
            ///
            bool was_node_error() const;
            
            ///
            /// Was the connection error caused by a network condition
            ///
            bool was_network_error() const;
            
            ///
            /// Returns the network error code that caused the connection to fail
            ///
            const boost::system::error_code& get_network_error() const;
            
            ///
            /// Returns the node errors that were encountered when trying to make
            /// a connection
            ///
            const node_error_list& get_node_errors() const;
            
            
        private:
            boost::system::error_code _singleError;
            node_error_list _nodeErrors;
        };
        
        
        
    }
}

#endif /* defined(__Project__connection_error__) */
