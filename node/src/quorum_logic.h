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

#ifndef __sopmq__quorum_logic__
#define __sopmq__quorum_logic__

#include "node.h"

#include <boost/noncopyable.hpp>

#include <vector>
#include <memory>
#include <array>
#include <functional>

namespace sopmq {
    namespace node {
        
        ///
        /// Encapsulates quorum retry and failure logic
        ///
        template <std::size_t RF, typename Context>
        class quorum_logic : public boost::noncopyable
        {
        public:
            typedef std::shared_ptr<quorum_logic> ptr;
            
            quorum_logic(const std::array<node::ptr, RF>& allNodes)
            : _all_nodes(allNodes)
            {
                
            }
            
            ///
            /// Whether or not given the current status of the quorum
            /// operation we can continue with trying a backup node
            ///
            bool can_continue() const
            {
                if (RF - _failed_nodes.size() > (RF / 2))
                {
                    return true;
                }
                
                return false;
            }
            
            ///
            /// Begins execution of the function under quorum logic
            ///
            void run()
            {
                for (int i = 0; i < RF - (RF / 2); ++i)
                {
                    _function(_all_nodes[i]);
                }
            }
            
            ///
            /// Sets the function that should be executed under quorum logic
            ///
            void set_function(std::function<void(node::ptr)> function)
            {
                _function = function;
            }
            
            ///
            /// Retrieves our context object
            ///
            Context& ctx()
            {
                return _ctx;
            }
            
            ///
            /// Marks an operation for a given node as successful
            ///
            void node_success(node::ptr node)
            {
                _success_nodes.push_back(node);
            }
            
            ///
            /// Marks an operation for a given node as failed
            ///
            void node_failed(node::ptr node)
            {
                _failed_nodes.push_back(node);
            }
            
            ///
            /// Returns whether or not a quorum of nodes have succeeded
            ///
            bool operation_succeeded() const
            {
                return _success_nodes.size() > RF / 2;
            }
            
            ///
            /// Returns the collection of nodes that had a success on the given operation
            ///
            std::vector<node::ptr>& successful_nodes()
            {
                return _success_nodes;
            }
            
        private:
            Context _ctx;
            std::array<node::ptr, RF> _all_nodes;
            std::function<void(node::ptr)> _function;
            
            std::vector<node::ptr> _success_nodes;
            std::vector<node::ptr> _failed_nodes;
        };
        
    }
}

#endif /* defined(__sopmq__quorum_logic__) */
