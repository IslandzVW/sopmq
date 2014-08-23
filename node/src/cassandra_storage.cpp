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
#include "cass_ptrs.h"
#include "cassandra_storage_async_helpers.h"

#include <boost/asio.hpp>

#include <string>
#include <sstream>
#include <array>
#include <functional>
#include <vector>

using std::string;

namespace sopmq {
    namespace node {
        namespace storage {
            
            const std::string& cassandra_storage::KEYSPACE_NAME = "sopmq";
            
            cassandra_storage::cassandra_storage()
            {
				_cluster = cass_cluster_new();

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
                const std::array<std::string, 2> CREATE_STATEMENTS =
                {
                    "CREATE KEYSPACE IF NOT EXISTS " + KEYSPACE_NAME +
                        " WITH REPLICATION = { 'class' : 'SimpleStrategy', 'replication_factor' : 3 };",
                    
                    "CREATE TABLE IF NOT EXISTS " + KEYSPACE_NAME + ".users (" +
                        "uname_hash varchar PRIMARY KEY, " +
                        "username varchar, " +
                        "pw_hash varchar, " +
                        "user_level int " +
                    ");"
                    
                };
                
                CassFuturePtr connect_future(cass_cluster_connect(_cluster));
                this->throw_on_error(connect_future.get());
                
                CassSession* session = cass_future_get_session(connect_future.get());
                
                for (auto statement : CREATE_STATEMENTS)
                {
                    this->execute_statement(session, statement);
                }
                
                /* Close the session */
                CassFuturePtr close_future(cass_session_close(session));
                cass_future_wait(close_future.get());
            }
            
            void cassandra_storage::execute_statement(CassSession* session, const std::string& statementText)
            {
                /* Build statement and execute */
                CassString query = cass_string_init(statementText.c_str());
                CassStatementPtr statement(cass_statement_new(query, 0));
                
                CassFuturePtr result_future(cass_session_execute(session, statement.get()));
                this->throw_on_error(result_future.get());
            }
            
            void cassandra_storage::throw_on_error(CassFuture* future)
            {
                if (cass_future_error_code(future) != CASS_OK) {
                    /* Handle error */
                    CassString message = cass_future_error_message(future);
                    throw storage_error(std::string(message.data, message.length));
                }
            }
            
            void cassandra_storage::create_user(const std::string& usernameHash, const std::string& username,
                                                const std::string& pwHash, int userLevel)
            {
                CassFuturePtr connect_future(cass_cluster_connect(_cluster));
                this->throw_on_error(connect_future.get());
                
                CassSession* session = cass_future_get_session(connect_future.get());
                
                std::string insert("INSERT INTO " + KEYSPACE_NAME + ".users"
                                   "(uname_hash, username, pw_hash, user_level) VALUES (?, ?, ?, ?);");
                CassString query = cass_string_init(insert.c_str());
                CassStatementPtr statement(cass_statement_new(query, 4));
                
                cass_statement_bind_string(statement.get(), 0, cass_string_init(usernameHash.c_str()));
                cass_statement_bind_string(statement.get(), 1, cass_string_init(username.c_str()));
                cass_statement_bind_string(statement.get(), 2, cass_string_init(pwHash.c_str()));
                cass_statement_bind_int32(statement.get(), 3, userLevel);
                
                CassFuturePtr result_future(cass_session_execute(session, statement.get()));
                this->throw_on_error(result_future.get());
                
                /* Close the session */
                CassFuturePtr close_future(cass_session_close(session));
                cass_future_wait(close_future.get());
            }
            
            std::string cassandra_storage::get_string_column_value(const CassRow* row, const std::string& colname)
            {
                const CassValue* value = cass_row_get_column_by_name(row, colname.c_str());
                CassString val_string;
                cass_value_get_string(value, &val_string);
                
                return std::string(val_string.data, val_string.length);
            }
            
            int cassandra_storage::get_int_column_value(const CassRow* row, const std::string& colname)
            {
                const CassValue* value = cass_row_get_column_by_name(row, colname.c_str());
                int ival;
                cass_value_get_int32(value, &ival);
                
                return ival;
            }
            
            void cassandra_storage::find_user(const std::string &usernameHash,
                                              std::function<void(const find_user_result&)> callback)
            {
                std::function<void(cassasync::async_query_state&)> cb = [=](cassasync::async_query_state& qstate)
                {
                    if (qstate.error)
                    {
                        find_user_result result;
                        result.error = std::move(qstate.error);
                        callback(result);
                    }
                    
                    
                    if (cass_iterator_next(qstate.rows.get()))
                    {
                        const CassRow* row = cass_iterator_get_row(qstate.rows.get());
                        
                        user_account acct(get_string_column_value(row, "uname_hash"),
                                          get_string_column_value(row, "username"),
                                          get_string_column_value(row, "pw_hash"),
                                          get_int_column_value(row, "level"));
                        
                        find_user_result fur;
                        fur.account = acct;
                    }
                    else
                    {
                        find_user_result fur;
                        fur.user_found = false;
                        callback(fur);
                    }
                };
                
                cassasync::async_query_state* qstate = new cassasync::async_query_state();
                qstate->query = "SELECT * FROM " + KEYSPACE_NAME + ".users WHERE uname_hash = ?";
                qstate->params.push_back(usernameHash);
                qstate->result_callback = cb;
                
                cassasync::async_query(qstate, _cluster);
            }
            
        }
    }
}