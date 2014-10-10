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

#ifndef __sopmq__operation_result__
#define __sopmq__operation_result__

#include <functional>

namespace sopmq {
    namespace node {
        namespace intra {
            
            ///
            /// The result of an intra-node operation
            ///
            template <typename ReturnMessageType>
            class operation_result
            {
            private:
                ReturnMessageType _message;
                std::function<void ()> _throw_function;
                
            public:
                operation_result(ReturnMessageType message)
                : _message(message), _throw_function()
                {
                    
                }
                
                operation_result(std::function<void ()> throwFunction)
                : _message(), _throw_function(throwFunction)
                {
                    
                }
                
                virtual ~operation_result()
                {
                    
                }
                
                ///
                /// The message that we got back in return for the operation
                ///
                ReturnMessageType message() const
                {
                    return _message;
                }
                
                ///
                /// Throws the error that occurred with this operation
                ///
                void rethrow_error() const
                {
                    if (_throw_function) _throw_function();
                }
            };
            
        }
    }
}

#endif /* defined(__sopmq__operation_status__) */
