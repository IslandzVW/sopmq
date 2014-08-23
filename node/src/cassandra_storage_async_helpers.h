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

#ifndef __sopmq__cassandra_storage_async_helpers__
#define __sopmq__cassandra_storage_async_helpers__

#include "storage_error.h"
#include "cass_ptrs.h"

#include <cassandra.h>

#include <string>
#include <functional>
#include <vector>
#include <memory>

///
/// Helper functions for cassandra's async query interface
///
namespace cassasync {
    
    ///
    /// The current state of an async query running against
    /// a cassandra datasource
    ///
    struct async_query_state
    {
        std::string query;
        std::vector<std::string> params;
        std::unique_ptr<sopmq::node::storage::storage_error> error;
        std::function<void(async_query_state&)> result_callback;
        
        sopmq::node::storage::CassFuturePtr connect_future;
        CassSession* session;
        sopmq::node::storage::CassStatementPtr statement;
        sopmq::node::storage::CassFuturePtr result_future;
        
        sopmq::node::storage::CassIteratorPtr rows;
        
        async_query_state()
        : session(nullptr)
        {
            
        }
    };
    
    ///
    /// Begins an async cassandra query with the given query state
    ///
    void async_query(async_query_state* qstate, CassCluster* cluster);
    
}


#endif /* defined(__sopmq__cassandra_storage_async_helpers__) */
