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

#ifndef __sopmq__netutil__
#define __sopmq__netutil__

#include <boost/asio.hpp>
#include <boost/pool/poolfwd.hpp>
#include <memory>
#include <cstdint>
#include <functional>

namespace sopmq {
    namespace util {
        
        struct message_context;
        
        
        ///
        /// Utility functions that deal with network related tasks
        ///
        class netutil
        {
        public:
            ///
            /// Reads an unsigned integer from the given socket
            ///
            static void read_u32(boost::asio::io_service& ioService,
                                 boost::asio::ip::tcp::socket& socket,
                                 std::function<void(uint32_t, const boost::system::error_code& error)> callback);
            
            ///
            /// Reads an unsigned short from the given socket
            ///
            static void read_u16(boost::asio::io_service& ioService,
                                 boost::asio::ip::tcp::socket& socket,
                                 std::function<void(uint16_t, const boost::system::error_code& error)> callback);
            
        private:
            static boost::pool<> s_mem_pool;
            
            static void after_u32_read(std::shared_ptr<char> buffer,
                                       std::function<void(uint32_t, const boost::system::error_code& error)> callback,
                                       const boost::system::error_code& error,
                                       std::size_t bytes_transferred);
            
            static void after_u16_read(std::shared_ptr<char> buffer,
                                       std::function<void(uint16_t, const boost::system::error_code& error)> callback,
                                       const boost::system::error_code& error,
                                       std::size_t bytes_transferred);
            
            static void free_mem(char* mem);
            
            netutil();
            ~netutil();
        };
    }
}

#endif /* defined(__sopmq__netutil__) */
