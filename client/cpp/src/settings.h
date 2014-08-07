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

#ifndef __client__settings__
#define __client__settings__

#include <string>
#include <vector>
#include <cstdint>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/noncopyable.hpp>

namespace bmp = boost::multiprecision;

namespace sopmq {
    namespace client {
        
        ///
        /// Settings singleton for a sopmq node
        ///
        class settings : public boost::noncopyable
        {
        public:
            static const uint32_t DEFAULT_MAX_MESSAGE_SIZE;
            
            ///
            /// Returns our singleton instance
            ///
            static settings& instance();
            
        public:
            ///
            /// The maximum message size that we allow
            ///
            uint32_t maxMessageSize;
            
            
        private:
            settings();
            ~settings();
        };
        
    }
}

#endif /* defined(__client__settings__) */
