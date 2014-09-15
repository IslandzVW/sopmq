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

#include "message_queue.h"
#include "vector_clock.h"
#include "node_clock.h"

using namespace sopmq::node;



TEST(MessageQueueTest, MessageQueueBasicOrdering)
{
    message_queue3 mq("abcde");
    
    node_clock a1 = {1, 1, 1};
    node_clock b1 = {2, 1, 1};
    node_clock c1 = {3, 1, 1};

    vector_clock3 m1;
    m1.value()[0] = a1;
    m1.value()[1] = b1;
    m1.value()[2] = c1;
}