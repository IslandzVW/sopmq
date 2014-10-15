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

#ifndef __sopmq__messageutil__
#define __sopmq__messageutil__

#include "message_types.h"
#include "network_error.h"
#include "message_dispatcher.h"
#include "Identifier.pb.h"
#include "network_operation_result.h"

#include <boost/asio.hpp>
#include <google/protobuf/message.h>
#include <boost/shared_array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/pool/pool.hpp>

#include <memory>
#include <functional>
#include <map>
#include <cstdint>

namespace sopmq {
    namespace message {

        ///
        /// Callback that will be fired after the completion of a network operation
        ///
        typedef std::function<void(const shared::net::network_operation_result&)> network_status_callback;
        
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
            network_status_callback status_callback;
            
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
        
        
        typedef std::unique_ptr<char[], void(*)(char*)> header_buf_ptr;

        ///
        /// Context when writing a message
        ///
        struct send_context
		{
			send_context(header_buf_ptr headerbuf, const std::string& messagebuf, network_status_callback status)
				: header_buf(std::move(headerbuf)), message_buf(messagebuf), status_callback(status)
			{
			}

			header_buf_ptr header_buf;
            std::string message_buf;
            network_status_callback status_callback;

		private: 
			send_context(const send_context&);
            send_context& operator =(const send_context&);
        };
        
        typedef std::shared_ptr<send_context> send_context_ptr;
        
        ///
        /// Utility functions that deal with network messages
        ///
        class messageutil
        {
        public:
            ///
            /// \brief Reads an unknown message type from the wire
            ///
            static void read_message(boost::asio::io_service& ioService,
                                     boost::asio::ip::tcp::socket& socket,
                                     network_status_callback statusCallback,
                                     message_dispatcher& dispatcher,
                                     uint32_t maxSize);
            
            ///
            /// \brief Writes a message to the wire
            ///
            static void write_message(message_type type,
                                      Message_ptr message,
                                      boost::asio::io_service& ioService,
                                      boost::asio::ip::tcp::socket& socket,
                                      network_status_callback statusCallback);
            
            ///
            /// \brief Builds a new identifier to tack onto a message
            ///
            static Identifier* build_id(std::uint32_t id, std::uint32_t inReplyTo);
            
            ///
            /// \brief Creates a new message in reply to the given message with the given id
            /// \tparam T The message type to create
            /// \param id The message identifier
            /// \param inReplyTo The identifier of the message we are replying to
            ///
            template <typename T>
            static std::shared_ptr<T> make_message(std::uint32_t id, std::uint32_t inReplyTo)
            {
                auto message = std::make_shared<T>();
                message->set_allocated_identity(messageutil::build_id(id, inReplyTo));
                
                return message;
            }
            
        private:
            static const int HEADER_SIZE;
            
            static boost::pool<> s_mem_pool;
            static void free_mem(char* mem);
            
            
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
            
            static void after_write_message(send_context_ptr ctx, const boost::system::error_code& error,
                                            size_t bytesTransferred);
            
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
                    auto e = sopmq::error::network_error("Unable to parse new message of type "
                                                         + boost::lexical_cast<std::string>(ctx->type)
                                                         + " message corrupted?");
                    
                    //error
                    ctx->status_callback(shared::net::network_operation_result(shared::net::ET_NETWORK, e));
                }
                else
                {
                    //dispatch
                    ctx->status_callback(shared::net::network_operation_result::success());
                    ctx->dispatcher.dispatch(message);
                }
            }
        };
        
    }
}


#endif /* defined(__sopmq__messageutil__) */
