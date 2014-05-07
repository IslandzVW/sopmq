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

#include "endpoint.h"

using namespace sopmq::node::net;

TEST(EndPointTest, EndPointParserIsSane)
{
    endpoint ep("sopmq1://localhost:1000");
    
    ASSERT_EQ(ep.port(), 1000);
    ASSERT_EQ(ep.protocol(), endpoint::SOPMQv1);
    ASSERT_EQ(ep.host_name(), "localhost");
}

TEST(EndPointTest, EndPointParserHandlesNoPort)
{
    endpoint ep("sopmq1://localhost");
    
    ASSERT_EQ(ep.port(), 8481);
    ASSERT_EQ(ep.protocol(), endpoint::SOPMQv1);
    ASSERT_EQ(ep.host_name(), "localhost");
}

TEST(EndPointTest, EndPointParserHandlesShortHost)
{
    endpoint ep("sopmq1://l");
    
    ASSERT_EQ(ep.port(), 8481);
    ASSERT_EQ(ep.protocol(), endpoint::SOPMQv1);
    ASSERT_EQ(ep.host_name(), "l");
}

TEST(EndPointTest, EndPointParserThrowsWithEmptyURI)
{
    ASSERT_THROW(endpoint ep(""), uri_parse_error);
}

TEST(EndPointTest, EndPointParserThrowsWithNoScheme)
{
    ASSERT_THROW(endpoint ep("sopmq"), uri_parse_error);
}

TEST(EndPointTest, EndPointParserThrowsWithOnlySchemeParts)
{
    ASSERT_THROW(endpoint ep("sopmq1:"), uri_parse_error);
    ASSERT_THROW(endpoint ep("sopmq1:/"), uri_parse_error);
}

TEST(EndPointTest, EndPointParserThrowsWithOnlyFullScheme)
{
    ASSERT_THROW(endpoint ep("sopmq1://"), uri_parse_error);
}

TEST(EndPointTest, EndPointParserThrowsWithInvalidScheme)
{
    ASSERT_THROW(endpoint ep("sopmq5://test"), invalid_protocol_error);
}