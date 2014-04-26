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

#include <string>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include "settings.h"

namespace bmp = boost::multiprecision;
namespace po = boost::program_options;

using namespace std;
using namespace sopmq::node;

const string& PRODUCT = "InWorldz SOPMQ";
const string& VERSION = "0.1";

const unsigned short DEFAULT_PORT = 8481;

int main(int argc, char* argv[])
{
    cout << PRODUCT << " v" << VERSION << endl;
    
    //read in options from the command line
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("range", po::value<bmp::uint128_t>(), "the range for this node to handle")
        ("bind_addr", po::value<string>(), "address to bind to")
        ("port", po::value<unsigned short>()->default_value(DEFAULT_PORT), "port to listen on (default: 8481)")
    ;
    
    try
    {
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
        
        if (vm.count("help")) {
            cout << desc << "\n";
            return 1;
        }
        
        if (vm["range"].empty() || vm["bind_addr"].empty() || vm["port"].empty())
        {
            cout << "range, bind_addr, and port options must be specified" << endl;
            return 1;
        }
        
        settings::instance().range = vm["range"].as<bmp::uint128_t>();
        settings::instance().bindAddress = vm["bind_addr"].as<string>();
        settings::instance().port = vm["port"].as<unsigned short>();
        
    }
    catch (const po::invalid_command_line_syntax& e)
    {
        cout << e.what() << endl;
    }
    
    
    
    return 0;
}

