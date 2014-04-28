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

TEST(EndPointTest, EndPointParserThrowsWithEmptyURI)
{
    ASSERT_THROW(endpoint ep(""), uri_parse_error);
}