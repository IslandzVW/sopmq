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

#include <string>
#include <sstream>

using std::string;

namespace sopmq {
    namespace node {
        namespace storage {
            
            const std::string& cassandra_storage::KEYSPACE_NAME = "sopmq";
            
            cassandra_storage::cassandra_storage()
            {
				CassError rc = CASS_OK;
				_cluster = cass_cluster_new();
				CassFuture* session_future = NULL;

				std::stringstream ipList;
				bool first = true;
				for (string ip : settings::instance().cassandraSeeds)
				{
					if (! first) ipList << ",";
					ipList << ip;

					first = false;
				}
                
                cass_cluster_set_contact_points(_cluster, ipList.str().c_str());
            }
            
            cassandra_storage::~cassandra_storage()
            {
				cass_cluster_free(_cluster);
            }
            
            cassandra_storage& cassandra_storage::instance()
            {
                static cassandra_storage instance;
                return instance;
            }
            
        }
    }
}