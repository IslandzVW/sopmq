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

PROJECT( sopmq-thirdparty )
cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Boost REQUIRED)
set(INCLUDES ${INCLUDES} ${Boost_INCLUDE_DIR})

file(GLOB LIB_SRCS src/*.h src/*.cpp src/MurmurHash3/*.h src/MurmurHash3/*.cpp)

include_directories(${INCLUDES})
add_library(sopmq-thirdparty ${LIB_SRCS})
