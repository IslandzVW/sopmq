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

#include "cluster_builder.h"

namespace sopmq {
    namespace client {
        
        cluster_builder::cluster_builder()
        {
            
        }
        
        cluster_builder::~cluster_builder()
        {
            
        }
        
        void cluster_builder::add_endpoint(const shared::net::endpoint &ep)
        {
            _endpoints.push_back(ep);
        }
        
        cluster::ptr cluster_builder::build() const
        {
            return std::make_shared<cluster>(_endpoints);
        }
        
    }
}