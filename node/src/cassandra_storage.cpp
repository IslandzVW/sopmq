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
            
            void cassandra_storage::init()
            {
                CassFuture* connect_future = cass_cluster_connect(_cluster);
                
                if (cass_future_error_code(connect_future) == CASS_OK) {
                    CassFuture* close_future = NULL;
                    CassSession* session = cass_future_get_session(connect_future);
                    
                    /* Build statement and execute query */
                    CassString query = cass_string_init("SELECT keyspace_name "
                                                        "FROM system.schema_keyspaces;");
                    CassStatement* statement = cass_statement_new(query, 0);
                    
                    CassFuture* result_future = cass_session_execute(session, statement);
                    
                    if(cass_future_error_code(result_future) == CASS_OK) {
                        /* Retrieve result set and iterator over the rows */
                        const CassResult* result = cass_future_get_result(result_future);
                        CassIterator* rows = cass_iterator_from_result(result);
                        
                        while(cass_iterator_next(rows)) {
                            const CassRow* row = cass_iterator_get_row(rows);
                            const CassValue* value = cass_row_get_column_by_name(row, "keyspace_name");
                            
                            CassString keyspace_name;
                            cass_value_get_string(value, &keyspace_name);
                            printf("keyspace_name: '%.*s'\n", (int)keyspace_name.length,
                                   keyspace_name.data);
                        }
                        
                        cass_result_free(result);
                        cass_iterator_free(rows);
                    } else {
                        /* Handle error */
                        CassString message = cass_future_error_message(result_future);
                        fprintf(stderr, "Unable to run query: '%.*s'\n", (int)message.length,
                                message.data);
                    } 
                    
                    cass_future_free(result_future);
                    
                    /* Close the session */
                    close_future = cass_session_close(session);
                    cass_future_wait(close_future);
                    cass_future_free(close_future);
                } else {
                    /* Handle error */
                    CassString message = cass_future_error_message(connect_future);
                    fprintf(stderr, "Unable to connect: '%.*s'\n", (int)message.length, 
                            message.data);
                }
                
                cass_future_free(connect_future);
            }
            
        }
    }
}