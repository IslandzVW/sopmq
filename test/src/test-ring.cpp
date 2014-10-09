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

#include "ring.h"
#include "endpoint.h"
#include "range_conflict_error.h"
#include "id_conflict_error.h"
#include "util.h"
#include "settings.h"

using namespace sopmq::node;
using namespace sopmq::shared::net;

TEST(RingTest, TestEmptyRingFind)
{
    ring r;
    ASSERT_EQ(r.find_primary_node_for_key(9999), node::ptr()) ;
}

TEST(RingTest, TestSingleNodeRingFind)
{
    ring r;

    node::ptr node(new sopmq::node::node(1, 0, endpoint("sopmq1://localhost:1")));
    r.add_node(node);
    
    ASSERT_EQ(r.find_primary_node_for_key(1024579), node);
}

TEST(RingTest, TestSingleNodeRingFindWithSameKey)
{
    ring r;
    
    node::ptr node(new sopmq::node::node(1, 20, endpoint("sopmq1://localhost:1")));
    r.add_node(node);
    
    ASSERT_EQ(r.find_primary_node_for_key(20), node) ;
}

TEST(RingTest, TestDoubleNodeRingFind)
{
    ring r;
    
    node::ptr node1(new sopmq::node::node(1, 10, endpoint("sopmq1://localhost:1")));
    node::ptr node2(new sopmq::node::node(2, 20, endpoint("sopmq1://localhost:2")));
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
    
    node::ptr node1(new sopmq::node::node(1, 10, endpoint("sopmq1://localhost:1")));
    node::ptr node2(new sopmq::node::node(2, 20, endpoint("sopmq1://localhost:2")));
    r.add_node(node1);
    r.add_node(node2);
    
    ASSERT_EQ(r.find_primary_node_for_key(10), node1);
    ASSERT_EQ(r.find_primary_node_for_key(20), node2);
}

TEST(RingTest, TestFindPrimaryAndSecondary)
{
    ring r;
    
    node::ptr node1(new sopmq::node::node(1, 10, endpoint("sopmq1://localhost:1")));
    node::ptr node2(new sopmq::node::node(2, 20, endpoint("sopmq1://localhost:2")));
    r.add_node(node1);
    r.add_node(node2);
    
    auto nodes = r.find_nodes_for_key(10);
    
    ASSERT_EQ(nodes[0], node1);
    ASSERT_EQ(nodes[1], node2);
    
    nodes = r.find_nodes_for_key(20);
    
    ASSERT_EQ(nodes[0], node2);
    ASSERT_EQ(nodes[1], node1);
    
    nodes = r.find_nodes_for_key(11);
    
    ASSERT_EQ(nodes[0], node1);
    ASSERT_EQ(nodes[1], node2);
    
    nodes = r.find_nodes_for_key(21);
    
    ASSERT_EQ(nodes[0], node2);
    ASSERT_EQ(nodes[1], node1);
}

TEST(RingTest, TestFindPrimaryAndSecondaryWith4Nodes)
{
    ring r;
    
    node::ptr node1(new sopmq::node::node(1, 10, endpoint("sopmq1://localhost:1")));
    node::ptr node2(new sopmq::node::node(2, 20, endpoint("sopmq1://localhost:2")));
    node::ptr node3(new sopmq::node::node(3, 30, endpoint("sopmq1://localhost:3")));
    node::ptr node4(new sopmq::node::node(4, 40, endpoint("sopmq1://localhost:4")));
    r.add_node(node1);
    r.add_node(node2);
    r.add_node(node3);
    r.add_node(node4);
    
    auto nodes = r.find_nodes_for_key(10);
    
    ASSERT_EQ(nodes[0], node1);
    ASSERT_EQ(nodes[1], node2);
    
    nodes = r.find_nodes_for_key(20);
    
    ASSERT_EQ(nodes[0], node2);
    ASSERT_EQ(nodes[1], node3);
    
    nodes = r.find_nodes_for_key(30);
    
    ASSERT_EQ(nodes[0], node3);
    ASSERT_EQ(nodes[1], node4);
    
    nodes = r.find_nodes_for_key(40);
    
    ASSERT_EQ(nodes[0], node4);
    ASSERT_EQ(nodes[1], node1);
    
    nodes = r.find_nodes_for_key(1);
    
    ASSERT_EQ(nodes[0], node4);
    ASSERT_EQ(nodes[1], node1);
    
    nodes = r.find_nodes_for_key(11);
    
    ASSERT_EQ(nodes[0], node1);
    ASSERT_EQ(nodes[1], node2);
    
    nodes = r.find_nodes_for_key(21);
    
    ASSERT_EQ(nodes[0], node2);
    ASSERT_EQ(nodes[1], node3);
    
    nodes = r.find_nodes_for_key(31);
    
    ASSERT_EQ(nodes[0], node3);
    ASSERT_EQ(nodes[1], node4);
    
    nodes = r.find_nodes_for_key(41);
    
    ASSERT_EQ(nodes[0], node4);
    ASSERT_EQ(nodes[1], node1);
}

TEST(RingTest, TestFindPrimarySecondaryAndTertiaryWith4Nodes)
{
    ring r;
    
    node::ptr node1(new sopmq::node::node(1, 10, endpoint("sopmq1://localhost:1")));
    node::ptr node2(new sopmq::node::node(2, 20, endpoint("sopmq1://localhost:2")));
    node::ptr node3(new sopmq::node::node(3, 30, endpoint("sopmq1://localhost:3")));
    node::ptr node4(new sopmq::node::node(4, 40, endpoint("sopmq1://localhost:4")));
    r.add_node(node1);
    r.add_node(node2);
    r.add_node(node3);
    r.add_node(node4);
    
    auto nodes = r.find_nodes_for_key(10);
    
    ASSERT_EQ(nodes[0], node1);
    ASSERT_EQ(nodes[1], node2);
    ASSERT_EQ(nodes[2], node3);
    
    nodes = r.find_nodes_for_key(20);
    
    ASSERT_EQ(nodes[0], node2);
    ASSERT_EQ(nodes[1], node3);
    ASSERT_EQ(nodes[2], node4);
    
    nodes = r.find_nodes_for_key(30);
    
    ASSERT_EQ(nodes[0], node3);
    ASSERT_EQ(nodes[1], node4);
    ASSERT_EQ(nodes[2], node1);
    
    nodes = r.find_nodes_for_key(40);
    
    ASSERT_EQ(nodes[0], node4);
    ASSERT_EQ(nodes[1], node1);
    ASSERT_EQ(nodes[2], node2);
    
    nodes = r.find_nodes_for_key(1);
    
    ASSERT_EQ(nodes[0], node4);
    ASSERT_EQ(nodes[1], node1);
    ASSERT_EQ(nodes[2], node2);
    
    nodes = r.find_nodes_for_key(11);
    
    ASSERT_EQ(nodes[0], node1);
    ASSERT_EQ(nodes[1], node2);
    ASSERT_EQ(nodes[2], node3);
    
    nodes = r.find_nodes_for_key(21);
    
    ASSERT_EQ(nodes[0], node2);
    ASSERT_EQ(nodes[1], node3);
    ASSERT_EQ(nodes[2], node4);
    
    nodes = r.find_nodes_for_key(31);
    
    ASSERT_EQ(nodes[0], node3);
    ASSERT_EQ(nodes[1], node4);
    ASSERT_EQ(nodes[2], node1);
    
    nodes = r.find_nodes_for_key(41);
    
    ASSERT_EQ(nodes[0], node4);
    ASSERT_EQ(nodes[1], node1);
    ASSERT_EQ(nodes[2], node2);
}

TEST(RingTest, TestIdConflict)
{
    ring r;
    
    node::ptr node1(new sopmq::node::node(1, 10, endpoint("sopmq1://localhost:1")));
    node::ptr node2(new sopmq::node::node(1, 20, endpoint("sopmq1://localhost:2")));
    r.add_node(node1);
    
    ASSERT_THROW(r.add_node(node2), sopmq::error::id_conflict_error);
}

TEST(RingTest, TestRangeConflict)
{
    ring r;
    
    node::ptr node1(new sopmq::node::node(1, 10, endpoint("sopmq1://localhost:1")));
    node::ptr node2(new sopmq::node::node(2, 10, endpoint("sopmq1://localhost:2")));
    r.add_node(node1);
    
    ASSERT_THROW(r.add_node(node2), sopmq::error::range_conflict_error);
}

TEST(RingTest, TestIsSelf)
{
    sopmq::node::settings::instance().nodeId = 1;
    sopmq::node::node n(1, 10, endpoint("sopmq1://localhost:1"));
    
    ASSERT_TRUE(n.is_self());
}