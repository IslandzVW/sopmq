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

#include "cluster_endpoint.h"

using sopmq::client::cluster_endpoint;
using sopmq::shared::net::endpoint;

TEST(ClusterEndPointTest, ClusterEndPointConstruction)
{
    endpoint ep("sopmq1://localhost");
    cluster_endpoint cep(ep);
    
    ASSERT_EQ(cep.network_endpoint().port(), 8481);
    ASSERT_EQ(cep.network_endpoint().protocol(), endpoint::SOPMQv1);
    ASSERT_EQ(cep.network_endpoint().host_name(), "localhost");
}

TEST(ClusterEndPointTest, ClusterEndPointFailureRecorded)
{
    endpoint ep("sopmq1://localhost");
    cluster_endpoint cep(ep);
    
    cep.mark_failed();
    
    ASSERT_GT(cep.secs_remining_before_retry(), boost::chrono::seconds(50));
    ASSERT_LT(cep.secs_since_last_try(), boost::chrono::seconds(5));
    ASSERT_TRUE(cep.is_failed());
    ASSERT_FALSE(cep.ready_for_retry());
}

TEST(ClusterEndPointTest, ClusterEndPointRecovery)
{
    endpoint ep("sopmq1://localhost");
    cluster_endpoint cep(ep);
    
    cep.mark_failed();
    ASSERT_FALSE(cep.ready_for_retry());
    cep.mark_up();
    ASSERT_TRUE(cep.ready_for_retry());
    ASSERT_FALSE(cep.is_failed());
}