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

#ifndef __sopmq__network_operation_result__
#define __sopmq__network_operation_result__

#include "network_error.h"

namespace sopmq {
    namespace net {
        
        ///
        /// The type of error that occurred during a network operation
        ///
        enum error_type
        {
            ///
            /// No error
            ///
            ET_NONE = 0,
            
            ///
            /// A timeout occurred before we received data
            ///
            ET_TIMEOUT,
            
            ///
            /// There was a network error (disconnect, etc)
            ///
            ET_NETWORK,
            
            ///
            /// There was an error parsing a network message
            ///
            ET_PARSE,
            
            ///
            /// We received a header indicating an invalid message type
            ///
            ET_INVALID_TYPE,
            
            ///
            /// The message we received was too large
            ///
            ET_TOO_LARGE
        };
        
        ///
        /// Used to pass back the result of a network operation to a caller
        ///
        class network_operation_result
        {
        public:
            ///
            /// Return a successful network operation result
            ///
            static const network_operation_result& success();
            
            ///
            /// Generate a network error result from a boost error code
            ///
            static network_operation_result from_error_code(const boost::system::error_code& e);
            
            ///
            /// Generate a network error result from a boost error code with additional information
            ///
            static network_operation_result from_error_code(const std::string& info,
                                                            const boost::system::error_code& e);
            
            
        public:
            explicit network_operation_result(error_type et);
            network_operation_result(error_type et, const error::network_error& error);
            virtual ~network_operation_result();
            
            ///
            /// Returns whether or not the operation was successful
            ///
            bool was_successful() const;
            
            ///
            /// Returns the type of error that occurred
            ///
            error_type get_error_type() const;
            
            ///
            /// Returns details about the error that occurred
            ///
            const error::network_error& get_error() const;
            
            ///
            /// If there was an error, it is rethrown here, otherwise does nothing
            ///
            void rethrow() const;
            
        private:
            network_operation_result();
            
            error_type _et;
            error::network_error _error;
        };
        
    }
}

#endif /* defined(__sopmq__network_operation_result__) */
