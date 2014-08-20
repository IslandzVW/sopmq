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

#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>

#include <string>
#include <exception>

#include "settings.h"
#include "cassandra_storage.h"

namespace po = boost::program_options;

using namespace std;
using namespace sopmq::node;
using sopmq::node::storage::cassandra_storage;

const string& PRODUCT = "InWorldz SOPMQ nodetool";
const string& VERSION = "0.1";

po::variables_map vm;

bool process_options(int argc, char* argv[])
{
    //read in options from the command line
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("init,i", "initialize cassandra data storage (requires storage_nodes)")
        ("storage_nodes", po::value<vector<string> >()->multitoken(), "list of cassandra nodes for data storage")
    ;
    
    try
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
        
        if (vm.count("help")) {
            cout << desc << "\n";
            return false;
        }
        
        if (vm.count("storage_nodes")) {
            settings::instance().cassandraSeeds = vm["storage_nodes"].as<vector<string> >();
        }
    }
    catch (const po::error& e)
    {
        BOOST_LOG_TRIVIAL(fatal) << "Error processing options: " << e.what();
        return false;
    }
    catch (const std::exception& e)
    {
        BOOST_LOG_TRIVIAL(fatal) << "Error processing options: " << e.what();
        return false;
    }
    
    return true;
}

void do_init()
{
    cassandra_storage::instance().init();
}

int main(int argc, char* argv[])
{
    BOOST_LOG_TRIVIAL(info) << PRODUCT << " v" << VERSION << endl;
    
    if (! process_options(argc, argv))
    {
        return 1;
    }
    
    if (vm.count("init"))
    {
        do_init();
    }
    
    return 0;
}

