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

#include "server.h"

#include "connection_in.h"

#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>
#include <boost/make_shared.hpp>

#include "network_error.h"
#include "logging.h"

namespace ba = boost::asio;
using sopmq::error::network_error;

namespace sopmq {
    namespace node {
        
        server::server(ba::io_service& ioService, unsigned short port)
        : _ioService(ioService), _port(port), _endpoint(ba::ip::tcp::v4(), _port),
        _acceptor(_ioService, _endpoint), _stopping(false)
        {
            BOOST_LOG_TRIVIAL(info) << "starting mq services on TCP/" << _port;
            
            //add ourselves to the ring
            _ring.add_node(node::get_self());
        }
        
        void server::start()
        {
            this->accept_new();
        }
        
        void server::accept_new()
        {
            connection::connection_in::ptr conn = std::make_shared<connection::connection_in>(_ioService, _ring);
            
            _acceptor.async_accept(conn->get_socket(),
                                   boost::bind(&server::handle_accept, this, conn,
                                               boost::asio::placeholders::error));
        }
        
        void server::stop()
        {
            _stopping = true;
            _acceptor.close();
        }
        
        void server::handle_accept(connection::connection_in::ptr conn, const boost::system::error_code& error)
        {
            if (_stopping) return;

            if (! error)
            {
                try {
                    conn->start(this);
                    
                } catch (const network_error& e) {
                    LOG_SRC(error) << "exception thrown when trying to start connection: " << e.what();
                    
                }
                
                this->accept_new();
            }
            else
            {
                LOG_SRC(error) << "error during accept(): " << error.message();
                this->accept_new();
            }
        }
        
        void server::connection_started(connection::connection_in::ptr conn)
        {
            _connections.insert(conn);
        }
        
        void server::connection_terminated(connection::connection_in::ptr conn)
        {
            _connections.erase(conn);
        }
        
    }
}