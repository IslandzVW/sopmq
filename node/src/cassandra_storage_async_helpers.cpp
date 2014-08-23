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

#include "cassandra_storage_async_helpers.h"

using namespace sopmq::node::storage;

namespace cassasync {

    void after_session_closed(CassFuture* f, void* data)
    {
        async_query_state* qstate = static_cast<async_query_state*>(data);
        cass_future_free(f);
        qstate->result_callback(*qstate);
    }
    
    bool check_async_future_error(CassFuture* f, async_query_state* qstate)
    {
        if (cass_future_error_code(f) != CASS_OK)
        {
            CassString message = cass_future_error_message(f);
            qstate->error.reset(new storage_error(std::string(message.data, message.length)));
            
            if (qstate->session == nullptr)
            {
                qstate->result_callback(*qstate);
                delete qstate;
            }
            else
            {
                //we need to close the session first
                CassFuture* session_close_future = cass_session_close(qstate->session);
                cass_future_set_callback(session_close_future, &after_session_closed, qstate);
            }
            
            return true;
        }
        
        return false;
    }

    void after_result(CassFuture* f, void* data)
    {
        async_query_state* qstate = static_cast<async_query_state*>(data);
        qstate->result_future.reset(f);
        
        if (check_async_future_error(f, qstate)) return;
        
        CassResultConstPtr result(cass_future_get_result(qstate->result_future.get()));
        
        qstate->rows.reset(cass_iterator_from_result(result.get()));
        
        qstate->result_callback(*qstate);
        
        //close the session
        CassFuture* session_close_future = cass_session_close(qstate->session);
        cass_future_set_callback(session_close_future, &after_session_closed, qstate);
    }
    
    void after_connect(CassFuture* f, void* data)
    {
        async_query_state* qstate = static_cast<async_query_state*>(data);
        qstate->connect_future.reset(f);
        
        if (check_async_future_error(f, qstate)) return;
        
        qstate->session = cass_future_get_session(f);
        
        CassString query = cass_string_init(qstate->query.c_str());
        qstate->statement.reset(cass_statement_new(query, qstate->params.size()));
        
        for (int i = 0; i < qstate->params.size(); ++i)
        {
            cass_statement_bind_string(qstate->statement.get(), i, cass_string_init(qstate->params[i].c_str()));
        }
        
        CassFuture* result_future = cass_session_execute(qstate->session, qstate->statement.get());
        cass_future_set_callback(result_future, &after_result, qstate);
    }
    
    void async_query(async_query_state* qstate, CassCluster* cluster)
    {
        CassFuture* connect_future = cass_cluster_connect(cluster);
        cass_future_set_callback(connect_future, &after_connect, qstate);
    }

}