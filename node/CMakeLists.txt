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

PROJECT( sopmq-node )
cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
#set(Boost_DEBUG 1)

set(DEFS ${DEFS} -DBOOST_ALL_DYN_LINK)

find_package(Boost COMPONENTS system program_options log thread REQUIRED)
set(INCLUDES ${INCLUDES} ${Boost_INCLUDE_DIR})
set(LIBS ${LIBS} ${Boost_LIBRARIES})

find_package(OpenSSL REQUIRED)
set(LIBS ${LIBS} ${OPENSSL_LIBRARIES})

find_package(LibCQL REQUIRED)
set(INCLUDES ${INCLUDES} ${LIBCQL_INCLUDE_DIR})
set(LIBS ${LIBS} ${LIBCQL_LIBRARIES})

add_definitions(${DEFS})
include_directories(${INCLUDES})
file(GLOB LIB_SRCS src/*.h src/*.cpp)
list(REMOVE_ITEM LIB_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/node-main.cpp)

#message(${LIB_SRCS})

add_library(sopmq-nodelib ${LIB_SRCS})
target_link_libraries(sopmq-nodelib ${LIBS})

add_executable(sopmq-node src/node-main.cpp)
target_link_libraries(sopmq-node ${LIBS} sopmq-nodelib)
