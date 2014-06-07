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

#include "cassandra_storage.h"
#include "settings.h"

#include <boost/asio.hpp>

#include <cql/cql.hpp>
#include <cql/cql_connection.hpp>
#include <cql/cql_session.hpp>
#include <cql/cql_cluster.hpp>
#include <cql/cql_builder.hpp>
#include <cql/cql_result.hpp>

#include <string>

using namespace cql;
using std::string;

namespace sopmq {
    namespace node {
        namespace storage {
            
            const std::string& cassandra_storage::KEYSPACE_NAME = "sopmq";
            
            cassandra_storage::cassandra_storage()
            {
                // initialize cql
                cql_initialize();
                
                //construct our cluster from global settings
                boost::shared_ptr<cql::cql_builder_t> builder = cql::cql_cluster_t::builder();
                
                for (string endpoint : settings::instance().cassandraSeeds)
                {
                    builder->add_contact_point(boost::asio::ip::address::from_string(endpoint));
                }
                
                //build the cluster
                _cluster = builder->build();
            }
            
            cassandra_storage::~cassandra_storage()
            {
                
            }
            
            cassandra_storage& cassandra_storage::instance()
            {
                static cassandra_storage instance;
                return instance;
            }
            
            boost::shared_ptr<cql_session_t> cassandra_storage::new_session()
            {
                return _cluster->connect(KEYSPACE_NAME);
            }
        }
    }
}