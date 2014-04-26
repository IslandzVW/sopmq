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
#include <exception>
#include <boost/program_options.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/log/trivial.hpp>
#include "settings.h"

namespace bmp = boost::multiprecision;
namespace po = boost::program_options;

using namespace std;
using namespace sopmq::node;

const string& PRODUCT = "InWorldz SOPMQ";
const string& VERSION = "0.1";

const unsigned short DEFAULT_PORT = 8481;

const string required_options[] = {"range", "bind_addr", "port"};

bool process_options(int argc, char* argv[])
{
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
            return false;
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
        
        settings::instance().range = vm["range"].as<bmp::uint128_t>();
        settings::instance().bindAddress = vm["bind_addr"].as<string>();
        settings::instance().port = vm["port"].as<unsigned short>();
        
    }
    catch (const po::error& e)
    {
        cerr << e.what() << endl;
        return false;
    }
    catch (const std::exception& e)
    {
        cerr << e.what() << endl;
        return false;
    }
    
    return true;
}

int main(int argc, char* argv[])
{
    BOOST_LOG_TRIVIAL(info) << PRODUCT << " v" << VERSION << endl;
    
    if (! process_options(argc, argv))
    {
        return 1;
    }
    
    
    
    return 0;
}

