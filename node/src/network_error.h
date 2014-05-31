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

#ifndef __Project__network_error__
#define __Project__network_error__

#include <stdexcept>

namespace sopmq {
    namespace error {
        
        class network_error : public std::runtime_error
        {
        public:
            explicit network_error(const std::string& what);
            virtual ~network_error();
        };
        
    }
}

#endif /* defined(__Project__network_error__) */
