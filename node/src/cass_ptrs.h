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
 *
 * \file
 * \brief Memory management routines for the cassandra C API classes
 */

#ifndef __sopmq__cass_ptrs__
#define __sopmq__cass_ptrs__

#include <cassandra.h>

namespace sopmq {
    namespace node {
        namespace storage {
            
            class CassFutureDeleter { public: void operator()(CassFuture* p) { if (p) cass_future_free(p); } };
            typedef std::unique_ptr<CassFuture, CassFutureDeleter> CassFuturePtr;
            
            class CassStatementDeleter { public: void operator()(CassStatement* p) { if (p) cass_statement_free(p); } };
            typedef std::unique_ptr<CassStatement, CassStatementDeleter> CassStatementPtr;
        }
    }
}



#endif /* defined(__sopmq__cass_ptrs__) */
