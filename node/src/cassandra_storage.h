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

#ifndef __sopmq__cassandra_storage__
#define __sopmq__cassandra_storage__

#include <cassandra.h>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include <memory>
#include <string>

#include "cass_ptrs.h"

namespace sopmq {
    namespace node {
        namespace storage {
            
            
            ///
            /// Encapsulates the cassandra storage cluster operations
            ///
            class cassandra_storage : public boost::noncopyable
            {
            public:
                ///
                /// \brief Retrieves the singleton instance
                ///
                static cassandra_storage& instance();
                
                ///
                /// \brief Called to initialize cassandra storage for the first time (synchronous)
                ///
                /// Should be called to initialize cassandra storage for the first time.
                /// Creates all the necessary tables for operation
                ///
                void init();
                
                ///
                /// \brief Creates a new user for the sopmq cluster (synchronous)
                ///
                void create_user(const std::string& usernameHash, const std::string& username,
                                 const std::string& pwHash, int userLevel);
                                 
            private:
				CassCluster* _cluster;

                ///
                /// \brief The name of our keyspace
                ///
                static const std::string& KEYSPACE_NAME;
                
                void execute_statement(CassSession* session, const std::string& statement);
                
                void throw_on_error(CassFuture* future);
                
                cassandra_storage();
                ~cassandra_storage();
            };
            
        }
    }
}

#endif /* defined(__sopmq__cassandra_storage__) */
