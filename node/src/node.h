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

#ifndef __Project__node__
#define __Project__node__

#include <boost/shared_ptr.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include <string>

#include "endpoint.h"

namespace sopmq {
    namespace node {
        
        ///
        /// Represents a node that we're aware of in our ring that can service
        /// requests. This could be the local node, or a remote node
        ///
        class node
        {
        private:
            ///
            /// The beginning of the range that we handle
            ///
            boost::multiprecision::uint128_t _rangeStart;
            
            ///
            /// The endpoint to contact this node at
            ///
            net::endpoint _endpoint;
            
        };
        
        typedef boost::shared_ptr<node> node_ptr;
    }
}

#endif /* defined(__Project__node__) */
