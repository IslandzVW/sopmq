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

#ifndef __Project__messageutil__
#define __Project__messageutil__

#include <boost/asio.hpp>
#include <google/protobuf/message.h>
#include <boost/shared_array.hpp>
#include <functional>

#include "message_types.h"
#include "network_error.h"

namespace sopmq {
    namespace util {
        
        struct message_context;
        
        ///
        /// Utility functions that deal with network messages
        ///
        class messageutil
        {
        public:
            typedef std::function<void(boost::shared_ptr<::google::protobuf::Message>,
                                       sopmq::messages::message_type messageType,
                                       const sopmq::error::network_error& error)>
            message_callback;
            
        public:
            ///
            /// Reads an unknown message type from the wire
            ///
            static void read_message(boost::asio::io_service& ioService,
                                     boost::asio::ip::tcp::socket& socket,
                                     message_callback callback,
                                     uint32_t maxSize);
            
        private:
            static void after_read_message_type(boost::asio::io_service& ioService,
                                                boost::asio::ip::tcp::socket& socket,
                                                message_context ctx,
                                                uint16_t messageType,
                                                const boost::system::error_code& error);
            
            static void after_read_message_size(boost::asio::io_service& ioService,
                                                boost::asio::ip::tcp::socket& socket,
                                                message_context ctx,
                                                uint32_t messageSize,
                                                const boost::system::error_code& error);
            
            static void after_read_message_content(boost::asio::io_service& ioService,
                                                   boost::asio::ip::tcp::socket& socket,
                                                   message_context ctx,
                                                   const boost::system::error_code& error,
                                                   std::size_t bytes_transferred);
        };
        
        
        
        ///
        /// Context for read_message
        ///
        struct message_context
        {
            messageutil::message_callback callback;
            uint32_t max_message_size;
            sopmq::messages::message_type type;
            uint32_t message_size;
            boost::shared_array<char> message_buffer;
        };
        
    }
}


#endif /* defined(__Project__messageutil__) */
