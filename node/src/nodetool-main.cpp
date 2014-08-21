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

#include "settings.h"

#include "cassandra_storage.h"
#include "storage_error.h"
#include "user_account.h"

#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include <boost/lexical_cast.hpp>

#include <string>
#include <vector>
#include <exception>

namespace po = boost::program_options;

using namespace std;
using namespace sopmq::node;
using sopmq::node::storage::cassandra_storage;
using sopmq::node::user_account;

const string& PRODUCT = "InWorldz SOPMQ nodetool";
const string& VERSION = "0.1";

po::variables_map vm;
po::options_description desc("Allowed options");

bool process_options(int argc, char* argv[])
{
    //read in options from the command line
    desc.add_options()
        ("help,h", "produce help message")
        ("init,i", "initialize cassandra data storage")
        ("create_user", "creates a new user for the cluster")
        ("storage_nodes", po::value<vector<string> >()->multitoken(), "list of cassandra nodes for data storage")
        ("args", po::value<vector<string> >()->multitoken())
    ;
    
    po::positional_options_description p;
    p.add("args", 3);
    
    try
    {
        po::store(po::command_line_parser(argc, argv)
                  .options(desc)
                  .positional(p)
                  .run(), vm);
        
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

int do_init()
{
    if (! vm.count("storage_nodes"))
    {
        BOOST_LOG_TRIVIAL(fatal) << "storage_nodes not specified";
        return 1;
    }
    
    try {
        cassandra_storage::instance().init();
        
    } catch (const sopmq::node::storage::storage_error& e) {
        BOOST_LOG_TRIVIAL(fatal) << "Unable to init cassandra storage: " << e.what();
        return 1;
    }
    
    return 0;
}

int do_create_user()
{
    if (! vm.count("storage_nodes"))
    {
        BOOST_LOG_TRIVIAL(fatal) << "storage_nodes not specified";
        return 1;
    }
    
    vector<string> args;
    if (!vm.count("args") || (args = vm["args"].as<vector<string> >()).size() != 3)
    {
        BOOST_LOG_TRIVIAL(fatal) << "usage: --create_user username password level";
        return 1;
    }
    
    int userLevel;
    try {
        userLevel = boost::lexical_cast<int>(args[2]);
    } catch (const boost::bad_lexical_cast& e) {
        BOOST_LOG_TRIVIAL(fatal) << "user level must be an integer";
        return 1;
    }
    
    try {
        user_account::create(args[0], args[1], userLevel);
    } catch (const std::exception& e) {
        BOOST_LOG_TRIVIAL(fatal) << "error creating user: " << e.what();
        return 1;
    }
    
    return 0;
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
        return do_init();
    }
    
    if (vm.count("create_user"))
    {
        return do_create_user();
    }
    
    cout << desc << "\n";
    
    return 1;
}

