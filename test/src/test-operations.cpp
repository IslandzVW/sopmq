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

#include "gtest/gtest.h"

#include "server.h"
#include "cluster_builder.h"
#include "endpoint.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <memory>
#include <functional>

using namespace sopmq::client;
using namespace sopmq::shared::net;

class OperationsTest : public ::testing::Test
{
protected:
    boost::thread* thread;
    boost::asio::io_service* ioService;
    sopmq::node::server* s;
    
    virtual void SetUp()
    {
        thread = new boost::thread(std::bind(&OperationsTest::do_run, this));
        boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
    }
    
    void do_run()
    {
        ioService = new boost::asio::io_service();
        s = new sopmq::node::server(*ioService, 8481);
        
        s->start();
        
        ioService->run();
    }
    
    virtual void TearDown()
    {
        s->stop();
    }
};

TEST_F(OperationsTest, TestAuthentication)
{
    /*
    boost::asio::io_service clientIoService;
    cluster_builder builder;
    builder.add_endpoint(endpoint("sopmq1://127.0.0.1:8481"));
    
    auto clstr = builder.build();
    
    auto authCb = [&](bool authd)
    {
        clientIoService.stop();
        ASSERT_TRUE(authd);
    };
    
    session::ptr mSession;
    auto connHandler = [&](session::ptr session, const sopmq::error::connection_error& e)
    {
        ASSERT_TRUE(session != nullptr) << e.what();
        
        //make sure session stays in scope for the tests
        mSession = session;
        session->authenticate("test", "test", authCb);
    };
    
    clstr->connect(clientIoService, connHandler);
    
    boost::asio::io_service::work work(clientIoService);
    clientIoService.run();
     */
}
