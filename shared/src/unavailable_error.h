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

#ifndef __sopmq__unavailable_error__
#define __sopmq__unavailable_error__

#include <stdexcept>

namespace sopmq {
    namespace error {
        
        ///
        /// Exception thrown when an operation can't be handled because the
        /// node or nodes that would handle it are not reachable
        ///
        class unavailable_error : public std::runtime_error
        {
        public:
            explicit unavailable_error(const std::string& what);
            
            virtual ~unavailable_error();
        };
        
    }
}

#endif /* defined(__sopmq__unavailable_error__) */
