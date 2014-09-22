# SOPMQ - Scalable optionally persistent message queue
# Copyright 2014 InWorldz, LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

file(GLOB PROTO_FILES *.proto)

#message(PROTO_FILES)

execute_process(COMMAND protoc -I=${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=cpp ${PROTO_FILES} 
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

#use cog to generate the message pointer types
execute_process(COMMAND cog.py -r ${CMAKE_CURRENT_SOURCE_DIR}/../shared/src/message_ptrs.h)

#also generate code for the message dispatcher
execute_process(COMMAND cog.py -r ${CMAKE_CURRENT_SOURCE_DIR}/../shared/src/message_dispatcher.h)
execute_process(COMMAND cog.py -r ${CMAKE_CURRENT_SOURCE_DIR}/../shared/src/message_dispatcher.cpp)

#and messageutil
execute_process(COMMAND cog.py -r ${CMAKE_CURRENT_SOURCE_DIR}/../shared/src/messageutil.cpp)
