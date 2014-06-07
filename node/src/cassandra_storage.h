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

#ifndef __Project__cassandra_storage__
#define __Project__cassandra_storage__

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <cql/cql_session.hpp>
#include <cql/cql_cluster.hpp>

#include <string>

namespace sopmq {
    namespace node {
        namespace storage {
            
            ///
            /// Encapsulates the cassandra storage cluster operations
            ///
            class cassandra_storage : public boost::noncopyable
            {
            public:
                static cassandra_storage& instance();
                
                ///
                /// Opens a new session with the cassandra cluster
                ///
                boost::shared_ptr<cql::cql_session_t> new_session();
                
            private:
                ///
                /// The name of our keyspace
                ///
                static const std::string& KEYSPACE_NAME;
                
                boost::shared_ptr<cql::cql_cluster_t> _cluster;
                
                cassandra_storage();
                ~cassandra_storage();
            };
            
        }
    }
}

#endif /* defined(__Project__cassandra_storage__) */
