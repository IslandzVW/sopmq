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

# - Try to find libcassandra (cassandra C++ driver)
# Once done this will define
#  LIBCASSANDRA_FOUND - System has LibCassandra
#  LIBCASSANDRA_INCLUDE_DIRS - The LibCassandra include directories
#  LIBCASSANDRA_LIBRARIES - The libraries needed to use LibCassandra
#  LIBCASSANDRA_DEFINITIONS - Compiler switches required for using LibCassandra


find_path(LIBCASSANDRA_INCLUDE_DIR cassandra.h
          HINTS
          PATH_SUFFIXES libcassandra )

find_library(LIBCASSANDRA_LIBRARY NAMES cassandra
             HINTS )


set(LIBCASSANDRA_LIBRARIES ${LIBCASSANDRA_LIBRARY} )
set(LIBCASSANDRA_INCLUDE_DIRS ${LIBCASSANDRA_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)

# handle the QUIETLY and REQUIRED arguments and set LIBCQL_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibCASSANDRA  DEFAULT_MSG
                                  LIBCASSANDRA_LIBRARY LIBCASSANDRA_INCLUDE_DIR)

mark_as_advanced(LIBCASSANDRA_INCLUDE_DIR LIBCASSANDRA_LIBRARY )
