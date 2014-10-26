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
#include "settings.h"
#include "user_account.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <memory>
#include <functional>

using namespace sopmq::client;
using namespace sopmq::shared::net;
using sopmq::node::settings;
using sopmq::node::user_account;

class OperationsTest : public ::testing::Test
{
protected:
    boost::thread* thread;
    boost::asio::io_service* ioService;
    sopmq::node::server* s;
    
    virtual void SetUp()
    {
        //if (settings::instance().cassandraSeeds.size() == 0) return;
        
        thread = new boost::thread(std::bind(&OperationsTest::do_run, this));
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
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
        //if (settings::instance().cassandraSeeds.size() == 0) return;
        
        s->stop();
    }
};

TEST_F(OperationsTest, TestAuthentication)
{
    if (settings::instance().cassandraSeeds.size() == 0) return;
    
    const char* const USERNAME = "test";
    const char* const PASSWORD = "test";
    
    user_account::create(USERNAME, PASSWORD, 1);
    
    boost::asio::io_service clientIoService;
    cluster_builder builder;
    builder.add_endpoint(endpoint("sopmq1://127.0.0.1:8481"));
    
    auto clstr = builder.build();
    
    bool authRan = false;
    
    auto authCb = [&](bool authd)
    {
        clientIoService.stop();
        authRan = true;
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
    
    ASSERT_TRUE(authRan);
}

TEST_F(OperationsTest, TestAuthenticationFailure)
{
    if (settings::instance().cassandraSeeds.size() == 0) return;
    
    const char* const USERNAME = "test";
    const char* const PASSWORD = "test";
    
    user_account::create(USERNAME, PASSWORD, 1);
    
    boost::asio::io_service clientIoService;
    cluster_builder builder;
    builder.add_endpoint(endpoint("sopmq1://127.0.0.1:8481"));
    
    auto clstr = builder.build();
    
    bool authRan = false;
    auto authCb = [&](bool authd)
    {
        clientIoService.stop();
        authRan = true;
        ASSERT_FALSE(authd);
    };
    
    session::ptr mSession;
    auto connHandler = [&](session::ptr session, const sopmq::error::connection_error& e)
    {
        ASSERT_TRUE(session != nullptr) << e.what();
        
        //make sure session stays in scope for the tests
        mSession = session;
        session->authenticate("testhjgasdh", "testdsadsadsa", authCb);
    };
    
    clstr->connect(clientIoService, connHandler);
    
    boost::asio::io_service::work work(clientIoService);
    clientIoService.run();
    
    ASSERT_TRUE(authRan);
}

TEST_F(OperationsTest, TestUnitTestAuthentication)
{
    boost::asio::io_service clientIoService;
    cluster_builder builder;
    builder.add_endpoint(endpoint("sopmq1://127.0.0.1:8481"));
    
    auto clstr = builder.build();
    
    bool authRan = false;
    
    auto authCb = [&](bool authd)
    {
        clientIoService.stop();
        authRan = true;
        ASSERT_TRUE(authd);
    };
    
    session::ptr mSession;
    auto connHandler = [&](session::ptr session, const sopmq::error::connection_error& e)
    {
        ASSERT_TRUE(session != nullptr) << e.what();
        
        //make sure session stays in scope for the tests
        mSession = session;
        session->authenticate(settings::instance().unitTestUsername, "", authCb);
    };
    
    clstr->connect(clientIoService, connHandler);
    
    boost::asio::io_service::work work(clientIoService);
    clientIoService.run();
    
    ASSERT_TRUE(authRan);
}
/*
TEST_F(OperationsTest, TestEnqueueMessage)
{
    boost::asio::io_service clientIoService;
    cluster_builder builder;
    builder.add_endpoint(endpoint("sopmq1://127.0.0.1:8481"));
    
    auto clstr = builder.build();
    
    bool authRan = false;
    
    session::ptr mSession;
    auto publishCb = [&](sopmq::shared::message::PublishMessageResponse pmr)
    {
        clientIoService.stop();
        authRan = true;
    };
    
    auto authCb = [&](bool authd)
    {
        ASSERT_TRUE(authd);
        
        mSession->publish_message("queue", false, 10, "Data", publishCb);
    };
    
    auto connHandler = [&](session::ptr session, const sopmq::error::connection_error& e)
    {
        ASSERT_TRUE(session != nullptr) << e.what();
        
        //make sure session stays in scope for the tests
        mSession = session;
        session->authenticate(settings::instance().unitTestUsername, "", authCb);
    };
    
    clstr->connect(clientIoService, connHandler);
    
    boost::asio::io_service::work work(clientIoService);
    clientIoService.run();
    
    ASSERT_TRUE(authRan);
}*/
