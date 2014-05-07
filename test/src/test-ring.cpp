#include "gtest/gtest.h"

#include "ring.h"
#include "endpoint.h"

#include <boost/multiprecision/random.hpp>

using namespace sopmq::node;
using namespace sopmq::node::net;
using namespace boost::multiprecision;
using namespace boost::random;

typedef independent_bits_engine<mt19937, 128, uint128_t> generator_type;
generator_type gen;

TEST(RingTest, TestEmptyRingFind)
{
    ring r;
    uint128_t num = gen();
    
    ASSERT_EQ(r.find_primary_node_for_key(num), node_ptr()) ;
}

TEST(RingTest, TestSingleNodeRingFind)
{
    ring r;
    uint128_t num = gen();
    
    node_ptr node(new sopmq::node::node(0, endpoint("sopmq1://localhost:1")));
    r.add_node(node);
    
    ASSERT_EQ(r.find_primary_node_for_key(num), node) ;
}

TEST(RingTest, TestSingleNodeRingFindWithSameKey)
{
    ring r;
    
    node_ptr node(new sopmq::node::node(20, endpoint("sopmq1://localhost:1")));
    r.add_node(node);
    
    ASSERT_EQ(r.find_primary_node_for_key(20), node) ;
}

TEST(RingTest, TestDoubleNodeRingFind)
{
    ring r;
    
    node_ptr node1(new sopmq::node::node(10, endpoint("sopmq1://localhost:1")));
    node_ptr node2(new sopmq::node::node(20, endpoint("sopmq1://localhost:2")));
    r.add_node(node1);
    r.add_node(node2);
    
    ASSERT_EQ(r.find_primary_node_for_key(1), node2);
    ASSERT_EQ(r.find_primary_node_for_key(21), node2);
    ASSERT_EQ(r.find_primary_node_for_key(11), node1);
    ASSERT_EQ(r.find_primary_node_for_key(19), node1);
}

TEST(RingTest, TestDoubleNodeRingFindWithSameKey)
{
    ring r;
    
    node_ptr node1(new sopmq::node::node(10, endpoint("sopmq1://localhost:1")));
    node_ptr node2(new sopmq::node::node(20, endpoint("sopmq1://localhost:2")));
    r.add_node(node1);
    r.add_node(node2);
    
    ASSERT_EQ(r.find_primary_node_for_key(10), node1);
    ASSERT_EQ(r.find_primary_node_for_key(20), node2);
}