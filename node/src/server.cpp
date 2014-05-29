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

#include "connection.h"

#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>

namespace ba = boost::asio;

namespace sopmq {
    namespace node {
        
        server::server(ba::io_service& ioService, unsigned short port)
        : _ioService(ioService), _port(port), _endpoint(ba::ip::tcp::v4(), _port),
        _acceptor(_ioService, _endpoint)
        {
            BOOST_LOG_TRIVIAL(info) << "starting mq services on TCP/" << _port;
        }
        
        void server::start()
        {
            this->accept_new();
        }
        
        void server::accept_new()
        {
            connection::connection::ptr conn(new connection::connection(_ioService));
            
            _acceptor.async_accept(conn->get_socket(),
                                   boost::bind(&server::handle_accept, this, conn,
                                               boost::asio::placeholders::error));
        }
        
        void server::handle_accept(connection::connection::ptr conn, const boost::system::error_code& error)
        {
            if (! error)
            {
                conn->start(this);
                this->accept_new();
            }
            else
            {
                BOOST_LOG_TRIVIAL(error) << "error during accept(): " << error.message();
                this->accept_new();
            }
        }
        
        void server::connection_started(connection::connection::ptr conn)
        {
            _connections.insert(conn);
        }
        
        void server::connection_terminated(connection::connection::ptr conn)
        {
            _connections.erase(conn);
        }
        
    }
}