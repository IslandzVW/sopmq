//
//  cassandra_storage.cpp
//  Project
//
//  Created by David Daeschler on 4/26/14.
//
//

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
using namespace boost;
using std::string;

namespace sopmq {
    namespace node {
        namespace storage {
            
            cassandra_storage::cassandra_storage()
            {
                // initialize cql
                cql_initialize();
                
                //construct our cluster from global settings
                shared_ptr<cql::cql_builder_t> builder = cql::cql_cluster_t::builder();
                
                for (string endpoint : settings::instance().cassandraSeeds)
                {
                    builder->add_contact_point(boost::asio::ip::address::from_string(endpoint));
                }
                
                // Now build a model of cluster and connect it to DB.
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
            
            shared_ptr<cql_session_t> cassandra_storage::new_session()
            {
                return _cluster->connect();
            }
        }
    }
}