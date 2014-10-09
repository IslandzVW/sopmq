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

///
/// Pointers for all message types
///

#ifndef Project_message_ptrs_h
#define Project_message_ptrs_h

#include <memory>
#include <google/protobuf/message.h>

typedef std::shared_ptr<::google::protobuf::Message> Message_ptr;

/*[[[cog
 
import cog
import glob
import os
 
ast = '*'
cog.outl("")

fnames = glob.glob("../messages/" + ast + ".proto")
for fn in fnames:
  rawname = os.path.splitext(os.path.basename(fn))[0]
  cog.outl("class %s;" % rawname)
  cog.outl("typedef std::shared_ptr<%s> %s_ptr;" % (rawname,rawname))
  cog.outl("");
 
cog.outl("");
]]]*/

class AnswerChallengeMessage;
typedef std::shared_ptr<AnswerChallengeMessage> AnswerChallengeMessage_ptr;

class AuthAckMessage;
typedef std::shared_ptr<AuthAckMessage> AuthAckMessage_ptr;

class ChallengeResponseMessage;
typedef std::shared_ptr<ChallengeResponseMessage> ChallengeResponseMessage_ptr;

class ConsumeFromQueueMessage;
typedef std::shared_ptr<ConsumeFromQueueMessage> ConsumeFromQueueMessage_ptr;

class ConsumeResponseMessage;
typedef std::shared_ptr<ConsumeResponseMessage> ConsumeResponseMessage_ptr;

class GetChallengeMessage;
typedef std::shared_ptr<GetChallengeMessage> GetChallengeMessage_ptr;

class GossipMessage;
typedef std::shared_ptr<GossipMessage> GossipMessage_ptr;

class GossipNodeData;
typedef std::shared_ptr<GossipNodeData> GossipNodeData_ptr;

class Identifier;
typedef std::shared_ptr<Identifier> Identifier_ptr;

class NodeClock;
typedef std::shared_ptr<NodeClock> NodeClock_ptr;

class PublishMessage;
typedef std::shared_ptr<PublishMessage> PublishMessage_ptr;

class PublishResponseMessage;
typedef std::shared_ptr<PublishResponseMessage> PublishResponseMessage_ptr;

class VectorClock;
typedef std::shared_ptr<VectorClock> VectorClock_ptr;


//[[[end]]]

#endif
