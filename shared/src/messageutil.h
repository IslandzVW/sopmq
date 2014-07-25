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
#include <memory>
#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>

#include <functional>
#include <map>
#include <cstdint>

#include "message_types.h"
#include "network_error.h"
#include "message_dispatcher.h"
#include "Identifier.pb.h"

namespace sopmq {
    namespace message {
        
        typedef std::function<void(const sopmq::error::network_error& error)> network_error_callback;
        
        ///
        /// Context for read_message
        ///
        struct message_context : public boost::noncopyable
        {
            ///
            /// Dispatcher to handle the message with
            ///
            message_dispatcher& dispatcher;
            
            ///
            /// Callback for error conditions
            ///
            network_error_callback error_callback;
            
            ///
            /// The maximum size for any message
            ///
            uint32_t max_message_size;
            
            ///
            /// The type of message we're handling
            ///
            sopmq::message::message_type type;
            
            ///
            /// Size of the message we're handling
            ///
            uint32_t message_size;
            
            ///
            /// Buffer for our message
            ///
            boost::shared_array<char> message_buffer;
            
            
            
            message_context(message_dispatcher& dispatcher)
            : dispatcher(dispatcher)
            {
            }
        };
        
        typedef std::shared_ptr<message_context> message_context_ptr;
        
        ///
        /// Utility functions that deal with network messages
        ///
        class messageutil
        {
        public:
            ///
            /// Reads an unknown message type from the wire
            ///
            static void read_message(boost::asio::io_service& ioService,
                                     boost::asio::ip::tcp::socket& socket,
                                     network_error_callback errorCallback,
                                     message_dispatcher& dispatcher,
                                     uint32_t maxSize);
            
            ///
            /// Builds a new identifier to tack onto a message
            ///
            static Identifier* build_id(std::uint32_t id, std::uint32_t inReplyTo);
            
            
        private:
            static void after_read_message_type(boost::asio::io_service& ioService,
                                                boost::asio::ip::tcp::socket& socket,
                                                message_context_ptr ctx,
                                                uint16_t messageType,
                                                const boost::system::error_code& error);
            
            static void after_read_message_size(boost::asio::io_service& ioService,
                                                boost::asio::ip::tcp::socket& socket,
                                                message_context_ptr ctx,
                                                uint32_t messageSize,
                                                const boost::system::error_code& error);
            
            static void after_read_message_content(boost::asio::io_service& ioService,
                                                   boost::asio::ip::tcp::socket& socket,
                                                   message_context_ptr ctx,
                                                   const boost::system::error_code& error,
                                                   std::size_t bytes_transferred);
            
            ///
            /// Decodes the message and then dispatches it
            ///
            static void switch_dispatch(message_context_ptr ctx);
            
            ///
            /// After the message type is decoded, we do the rest of the work here
            ///
            template <typename T>
            static void template_dispatch(message_context_ptr ctx, T message)
            {
                if (! message->ParseFromArray(ctx->message_buffer.get(), ctx->message_size))
                {
                    //error
                    ctx->error_callback(sopmq::error::network_error("Unable to parse new message of type "
                                                                    + boost::lexical_cast<std::string>(ctx->type)
                                                                    + " message corrupted?"));
                }
                else
                {
                    //dispatch
                    ctx->dispatcher.dispatch(message);
                }
            }
        };
        
    }
}


#endif /* defined(__Project__messageutil__) */
