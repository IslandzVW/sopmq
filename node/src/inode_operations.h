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


#ifndef __sopmq__inode_operations__
#define __sopmq__inode_operations__

namespace sopmq {
    namespace node {
        namespace intra {
            
            ///
            /// Interface to intranode operations. Used to simplify the implementation of
            /// executing operations between nodes on the ring
            ///
            class inode_operations
            {
            public:
                
                //virtual void gossip(
                
                inode_operations();
                virtual ~inode_operations();
            };
            
        }
    }
}

#endif /* defined(__sopmq__inode_comms__) */
