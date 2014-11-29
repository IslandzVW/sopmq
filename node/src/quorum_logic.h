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
            : _all_nodes(allNodes), _lastNode(0)
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
                    if (_lastNode + 1 < RF)
                    {
                        return true;
                    }
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
                    _function(_all_nodes[_lastNode++]);
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
                
                if (can_continue())
                {
                    _function(_all_nodes[_lastNode++]);
                }
                else
                {
                    _fail_function();
                }
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
            
            ///
            /// A function to be called when a quorum can not be reached
            ///
            void set_fail_function(std::function<void()> failFunc)
            {
                _fail_function = failFunc;
            }
            
        private:
            Context _ctx;
            std::array<node::ptr, RF> _all_nodes;
            std::function<void(node::ptr)> _function;
            std::function<void()> _fail_function;
            
            std::vector<node::ptr> _success_nodes;
            std::vector<node::ptr> _failed_nodes;
            
            int _lastNode;
        };
        
    }
}

#endif /* defined(__sopmq__quorum_logic__) */
