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

#include "authenticated_state.h"

#include "session.h"
#include "PublishMessage.pb.h"
#include "PublishResponseMessage.pb.h"
#include "util.h"
#include "messageutil.h"
#include "logging.h"
#include "responses.h"

#include <functional>

using sopmq::shared::util;
using sopmq::message::messageutil;
using sopmq::shared::message::PublishMessageResponse;

using namespace std::placeholders;

namespace sopmq {
    namespace client {
        namespace impl {
            
            authenticated_state::authenticated_state(cluster_connection::ptr conn, session::wptr session)
            : _conn(conn), _session(session),
            _dispatcher(std::bind(&authenticated_state::on_unhandled_message, this, _1, _2))
            {
                
            }
            
            authenticated_state::~authenticated_state()
            {
                
            }
            
            void authenticated_state::on_unhandled_message(Message_ptr message, const std::string& typeName)
            {
                LOG_SRC(error) << _conn->endpoint()
                    << " protocol violation: unexpected message: "
                    << typeName;
                
                if (auto session = _session.lock())
                {
                    session->protocol_violation();
                }
            }
            
            void authenticated_state::state_entry()
            {
                
            }
            
            void authenticated_state::publish_message(const std::string& queueId, bool storeIfCantPipe, int ttl,
                                                      const std::string& data, publish_message_callback callback)
            {
                PublishMessage_ptr message = messageutil::make_message<PublishMessage>(_conn->get_next_id(), 0);
                message->set_allocated_message_id(util::uuid_to_bytes(util::random_uuid()));
                message->set_queue_id(queueId);
                message->set_ttl(ttl);
                message->set_content(data);
                
                
                if (storeIfCantPipe) message->set_flags(PublishMessage::Flags::PublishMessage_Flags_STORE_IF_PIPE_FAILS);
                else message->set_flags(0);
                
                auto self(shared_from_this());
                _conn->send_message(sopmq::message::message_type::MT_PUBLISH, message,
                                    [=] (const shared::net::network_operation_result& result)
                                    {
                                        if (result.was_successful())
                                        {
                                            std::function<void(PublishResponseMessage_ptr)> responseHandler =
                                                [=] (PublishResponseMessage_ptr response)
                                                {
                                                    //we have a response
                                                    callback((PublishMessageResponse) response->status());
                                                };
                                            
                                            _dispatcher.set_handler(responseHandler, message->identity().id());
                                            
                                            self->_conn->read_message(_dispatcher,
                                                                      [=] (const shared::net::network_operation_result& result)
                                                                      {
                                                                          if (! result.was_successful())
                                                                          {
                                                                              if (auto session = self->_session.lock())
                                                                              {
                                                                                  session->connection_error(result);
                                                                              }
                                                                          }
                                                                      });
                                        }
                                        else
                                        {
                                            if (auto session = self->_session.lock())
                                            {
                                                session->connection_error(result);
                                            }
                                        }
                                    });
            }
            
        }
    }
}