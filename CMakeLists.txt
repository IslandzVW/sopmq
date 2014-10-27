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

cmake_minimum_required(VERSION 2.8)

project( sopmq )

if (WIN32)
	SET(gtest_force_shared_crt ON CACHE BOOL
  		"Use shared (DLL) run-time lib even when Google Test is built as static lib.")

	ADD_DEFINITIONS(-DWIN32 -D_WIN32 -D_WINDOWS)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS(-D_WIN32_WINNT=0x0601)
	ADD_DEFINITIONS(-D_VARIADIC_MAX=10)
	if ( HAS_64_BIT )
		ADD_DEFINITIONS(-DWIN64 -D_WIN64)
	endif ( HAS_64_BIT )

	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else (WIN32)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif (WIN32)

#ADD_DEFINITIONS(-DCRYPTOPP_IMPORTS)

set(NODE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/node)
set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/src)
set(SHARED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shared)
set(CLIENT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/client/cpp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(CryptoPP REQUIRED)

add_subdirectory(thirdparty)
add_subdirectory(messages)
add_subdirectory(shared)
add_subdirectory(node)
add_subdirectory(client/cpp)
add_subdirectory(gtest-1.7.0)
add_subdirectory(test)

enable_testing ()
add_test (NAME NodeTest COMMAND Test)
