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

#include "endpoint.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <vector>
#include <string>
#include <sstream>

using std::vector;
using std::string;
using std::stringstream;

namespace ba_ip = boost::asio::ip;

namespace sopmq {
    namespace shared {
        namespace net {
            
            endpoint::endpoint()
            {
                
            }
            
            endpoint::endpoint(const std::string& uri)
            {
                this->parse_uri(uri);
                _stringUri = uri;
            }
            
            endpoint::endpoint(const ba_ip::tcp::endpoint& ep)
            : _hostname(ep.address().to_string()), _port(ep.port()), _proto(SOPMQv1)
            {
            }
            
            endpoint::endpoint(const std::string& hostName, unsigned short port)
            : _hostname(hostName), _port(port), _proto(SOPMQv1)
            {
                
            }

            endpoint::~endpoint()
            {
                
            }
            
            void endpoint::parse_uri(const std::string &uri)
            {
                //  sopmq1://hostname:port
                //  [    ]://[      ]:[  ]
                
                auto iter = uri.begin();
                auto end = uri.end();
                
                if (iter == end) throw uri_parse_error("uri was empty");
                
                stringstream scheme;
                //read everything until the :
                while (iter != end && *iter != ':')
                {
                    scheme << *iter;
                    ++iter;
                }
                
                if (iter == end) throw uri_parse_error("scheme could not be deduced");
                
                ++iter;
                
                //next should be the first slash
                if (iter == end ||(*iter) != '/') throw uri_parse_error("expecting a / after scheme:");
                
                ++iter;
                
                //next should be the second slash
                if (iter == end ||(*iter) != '/') throw uri_parse_error("expecting a / after scheme:/");
                
                ++iter;
                
                if (iter == end) throw uri_parse_error("nothing found after scheme");
                
                stringstream hostname;
                //read everything until the : or the end of the uri string
                while (iter != end && *iter != ':')
                {
                    hostname << *iter;
                    ++iter;
                }
                
                //if we're at the end here, it means that there is no port specified,
                //so we use the default for the scheme
                if (iter == end)
                {
                    string sscheme = scheme.str();
                    string shostname = hostname.str();
                    
                    _proto = this->scheme_name_to_protocol(sscheme);
                    _hostname = shostname;
                    _port = this->get_default_port_for_proto(_proto);
                    return;
                }
                
                ++iter;
                
                if (iter == end) throw uri_parse_error("port not specified");
                
                //if not, we need to read the port
                stringstream port;
                //read everything until the end of the uri string
                while (iter != end)
                {
                    port << *iter;
                    ++iter;
                }
                
                string sscheme = scheme.str();
                string shostname = hostname.str();
                
                _proto = this->scheme_name_to_protocol(sscheme);
                _hostname = shostname;
                
                
                try {
                    _port = boost::lexical_cast<unsigned short>(port.str());
                    
                } catch (const boost::bad_lexical_cast&) {
                    throw uri_parse_error("port number specified was invalid");
                }
                
            }
            
            unsigned short endpoint::get_default_port_for_proto(endpoint_proto proto) const
            {
                switch (proto)
                {
                    case SOPMQv1:
                        return 8481;
                        
                    default:
                        throw invalid_protocol_error("port number for protocol is unknown");
                }
            }
            
            endpoint::endpoint_proto endpoint::scheme_name_to_protocol(const std::string& scheme) const
            {
                if (boost::algorithm::to_lower_copy(scheme) == "sopmq1")
                {
                    return SOPMQv1;
                }
                
                throw invalid_protocol_error(scheme + " is an unknown protocol");
            }
            
            const std::string& endpoint::host_name() const
            {
                return _hostname;
            }
            
            unsigned short endpoint::port() const
            {
                return _port;
            }
            
            endpoint::endpoint_proto endpoint::protocol() const
            {
                return _proto;
            }
            
            const string& endpoint::str() const
            {
                return _stringUri;
            }
            
            bool operator < (const endpoint& lhs, const endpoint& rhs)
            {
                return lhs.str() < rhs.str();
            }
            
            std::ostream& operator<< (std::ostream& stream, const endpoint& ep)
            {
                return stream << ep.str();
            }
            
        }
    }
}