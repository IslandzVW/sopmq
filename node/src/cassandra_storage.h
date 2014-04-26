//
//  cassandra_storage.h
//  Project
//
//  Created by David Daeschler on 4/26/14.
//
//

#ifndef __Project__cassandra_storage__
#define __Project__cassandra_storage__

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <cql/cql_session.hpp>
#include <cql/cql_cluster.hpp>

namespace sopmq {
    namespace node {
        namespace storage {
            
            ///
            /// Encapsulates the cassandra storage cluster
            ///
            class cassandra_storage : public boost::noncopyable
            {
            private:
                boost::shared_ptr<cql::cql_cluster_t> _cluster;
                
                cassandra_storage();
                ~cassandra_storage();
                
            public:
                static cassandra_storage& instance();
                
                ///
                /// Opens a new session with the cassandra cluster
                ///
                boost::shared_ptr<cql::cql_session_t> new_session();
            };
            
        }
    }
}

#endif /* defined(__Project__cassandra_storage__) */
