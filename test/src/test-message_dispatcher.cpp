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

#include "message_dispatcher.h"
#include "GetChallengeMessage.pb.h"
#include "network_operation_result.h"

#include <memory>

using namespace sopmq::message;
using namespace sopmq::shared::net;

TEST(MessageDispatcherTest, TestUnhandledHandler)
{
    bool called = false;
    
    message_dispatcher md([&](Message_ptr msg, const std::string&) { called = true; });
    
    GetChallengeMessage_ptr gcm = std::make_shared<GetChallengeMessage>();
    md.dispatch(network_operation_result::success(), gcm);
    
    ASSERT_TRUE(called);
}

TEST(MessageDispatcherTest, TestRegisteredHandler)
{
    bool unhandled = true;
    bool called = false;
    
    message_dispatcher md([&](Message_ptr msg, const std::string&) { unhandled = true; });

	std::function<void(const network_operation_result&, GetChallengeMessage_ptr)> func
        = [&](const network_operation_result&, GetChallengeMessage_ptr msg) { unhandled = false; called = true; };
    
    md.set_handler(func);
    
    GetChallengeMessage_ptr gcm = std::make_shared<GetChallengeMessage>();
    
    md.dispatch(network_operation_result::success(), gcm);
    
    ASSERT_TRUE(called);
    ASSERT_FALSE(unhandled);

}

TEST(MessageDispatcherTest, TestRegisteredHandlerWithReplyTo)
{
    const uint32_t replyTo = 2;
    
    bool unhandled = true;
    bool called = false;
    
    
    message_dispatcher md([&](Message_ptr msg, const std::string&) { unhandled = true; });
    
    std::function<void(const network_operation_result&, GetChallengeMessage_ptr)> func
        = [&](const network_operation_result&, GetChallengeMessage_ptr msg) { unhandled = false; called = true; };
    md.set_handler(func, replyTo);
    
    GetChallengeMessage_ptr gcm = std::make_shared<GetChallengeMessage>();
    gcm->mutable_identity()->set_in_reply_to(replyTo);
    
    md.dispatch(network_operation_result::success(), gcm);
    
    ASSERT_TRUE(called);
    ASSERT_FALSE(unhandled);
    
}

TEST(MessageDispatcherTest, TestRegisteredHandlerUnhandledReplyTo)
{
    const uint32_t replyTo = 2;
    
    bool unhandled = true;
    bool called = false;
    
    
    message_dispatcher md([&](Message_ptr msg, const std::string&) { unhandled = true; });
    
    std::function<void(const network_operation_result&, GetChallengeMessage_ptr)> func
        = [&](const network_operation_result&, GetChallengeMessage_ptr msg) { unhandled = false; called = true; };
    md.set_handler(func);
    
    GetChallengeMessage_ptr gcm = std::make_shared<GetChallengeMessage>();
    gcm->mutable_identity()->set_in_reply_to(replyTo);
    
    md.dispatch(network_operation_result::success(), gcm);
    
    ASSERT_FALSE(called);
    ASSERT_TRUE(unhandled);
    
}