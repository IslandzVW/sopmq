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

#include "failure_detector.h"
#include <boost/chrono/duration.hpp>
#include <boost/thread.hpp>

using sopmq::node::failure_detector;
namespace bc = boost::chrono;

TEST(FailureDetectorTest, PhiGreaterOrEqual)
{
    bc::milliseconds init(100);
    failure_detector fd(1, init);

    auto later = bc::steady_clock::now() + (init * 3);

    ASSERT_EQ(failure_detector::DOWN, fd.interpret(later));
}

TEST(FailureDetectorTest, PhiUnder)
{
    bc::milliseconds init(100);
    failure_detector fd(1, init);

    auto later = bc::steady_clock::now() + init;

    ASSERT_EQ(failure_detector::UP, fd.interpret(later));
}