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
#include "server.h"
#include "uint128.h"

#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include <boost/asio.hpp>

#include <string>
#include <exception>
#include <fstream>
#include <cstdint>

namespace po = boost::program_options;

using namespace std;
using namespace sopmq::node;

const string& PRODUCT = "InWorldz SOPMQ";
const string& VERSION = "0.1";

const uint16_t DEFAULT_PORT = 8481;
const uint32_t DEFAULT_MAX_MESSAGE_SIZE = 10485760;
const int DEFAULT_PHI_FAILURE_THRESHOLD = 7;

const string required_options[] = {"range", "bind_addr", "port", "storage_nodes", "seed_nodes"};

bool process_options(int argc, char* argv[])
{
    //read in options from the command line
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("config_file", po::value<string>()->default_value(""), "path to configuration file")
        ("range", po::value<string>(), "the start of the range for this node to handle")
        ("bind_addr", po::value<string>(), "address to bind to")
        ("port", po::value<uint16_t>()->default_value(DEFAULT_PORT), "port to listen on")
        ("storage_nodes", po::value<vector<string> >()->multitoken(), "list of cassandra nodes for data storage")
        ("seed_nodes", po::value<vector<string> >()->multitoken(), "list of seed nodes to get us into the ring")
        ("max_message_size", po::value<uint32_t>()->default_value(DEFAULT_MAX_MESSAGE_SIZE), "the maximum size of any message")
        ("phi_failure_threshold", po::value<int>()->default_value(DEFAULT_PHI_FAILURE_THRESHOLD), "the default threshold at which we consider a node failed")
        ("unit_test_username", po::value<string>()->default_value(""), "Username that will be allowed to log in unconditionally to perform unit testing")
    ;
    
    try
    {
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
        
        if (vm.count("help")) {
            cout << desc << "\n";
            return false;
        }
        
        string configFile;
        
        if (! vm["config_file"].empty())
        {
            configFile = vm["config_file"].as<string>();
        }
        
        if (! configFile.empty())
        {
            //use the config file
            ifstream ifs(configFile);
            if (!ifs)
            {
                BOOST_LOG_TRIVIAL(fatal) << "can not open config file: " << configFile << "\n";
                return 0;
            }
            else
            {
                po::store(po::parse_config_file(ifs, desc), vm);
                notify(vm);
            }
        }
        
        bool wasMissingOption = false;
        for (string opt : required_options)
        {
            if (vm[opt].empty())
            {
                BOOST_LOG_TRIVIAL(fatal) << "Option " << opt << " must be defined";
                wasMissingOption = true;
            }
        }
        
        if (wasMissingOption)
        {
            return false;
        }
        
        settings::instance().range = uint128(vm["range"].as<std::string>());
        settings::instance().bindAddress = vm["bind_addr"].as<string>();
        settings::instance().port = vm["port"].as<unsigned short>();
        settings::instance().cassandraSeeds = vm["storage_nodes"].as<vector<string> >();
        settings::instance().mqSeeds = vm["seed_nodes"].as<vector<string> >();
        settings::instance().maxMessageSize = vm["max_message_size"].as<uint32_t>();
        settings::instance().phiFailureThreshold = vm["phi_failure_threshold"].as<int>();
        settings::instance().unitTestUsername = vm["unit_test_username"].as<string>();
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

void print_option_summary()
{
    BOOST_LOG_TRIVIAL(info) << "range: " << settings::instance().range;
    BOOST_LOG_TRIVIAL(info) << "bind_address: " << settings::instance().bindAddress;
    BOOST_LOG_TRIVIAL(info) << "port: " << settings::instance().port;
    
    for (string seed : settings::instance().cassandraSeeds)
    {
        BOOST_LOG_TRIVIAL(info) << "storage: " << seed;
    }
}

int main(int argc, char* argv[])
{
    BOOST_LOG_TRIVIAL(info) << PRODUCT << " v" << VERSION << endl;
    
    if (! process_options(argc, argv))
    {
        return 1;
    }
    
    print_option_summary();
    
    boost::asio::io_service ioService;
    
    sopmq::node::server s(ioService, settings::instance().port);
    
    s.start();
    
    ioService.run();
    
    return 0;
}

