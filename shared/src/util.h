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

#ifndef __sopmq__util__
#define __sopmq__util__

#include "uint128.h"

#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/multiprecision/cpp_int.hpp>

namespace sopmq {
    namespace shared {
        
        ///
        /// General utility functions
        ///
        class util
        {
        public:
            ///
            /// Takes a string of bytes and returns a human readable hex encoding
            ///
            static std::string hex_encode(const unsigned char* input, int inputSize);
        
            ///
            /// Generates a string of random bytes
            /// \param count The number of random bytes to generate
            ///
            static std::string random_bytes(int count);
            
            ///
            /// Generates a random uuid
            ///
            static boost::uuids::uuid random_uuid();
            
            ///
            /// Returns the murmur hash of the given value
            ///
            static uint128 murmur_hash3(const void * key, int len);
            
            ///
            /// Returns the murmur hash of the given value
            ///
            static uint128 murmur_hash3(const std::string& value);
            
            ///
            /// Extracts the raw bytes from a uuid. The string must be freed by the caller.
            /// Useful for inserting UUIDs into network messages
            ///
            static std::string* uuid_to_bytes(const boost::uuids::uuid& id);
            
            ///
            /// Constructs a uuid from a set of bytes
            ///
            static boost::uuids::uuid uuid_from_bytes(const std::string& bytes);
            
            ///
            /// Computes the sha256 of the given data and returns a human readable hex string
            /// result
            ///
            static std::string sha256_hex_string(const std::string& data);
            

        private:
            util();
            ~util();
        };
    }
}

#endif /* defined(__sopmq__util__) */
