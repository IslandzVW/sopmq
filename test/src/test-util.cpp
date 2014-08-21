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

#include "util.h"

#include <string>

using sopmq::shared::util;

TEST(UtilTest, HexEncoderWorks)
{
    std::string testStr("abcde");
    
    std::string result = util::hex_encode((const unsigned char*)testStr.c_str(), testStr.size());
    
    ASSERT_EQ("6162636465", result);
}

TEST(UtilTest, RandomBytes)
{
    std::string bytes = util::random_bytes(1024);
    ASSERT_EQ(1024, bytes.length());
}