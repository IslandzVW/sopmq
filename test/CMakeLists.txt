find_package(Boost COMPONENTS system program_options log thread REQUIRED)

find_package(Protobuf REQUIRED)
set(INCLUDES ${INCLUDES} ${PROTOBUF_INCLUDE_DIR})
set(LIBS ${LIBS} ${PROTOBUF_LIBRARIES})

set(INCLUDES ${INCLUDES} ../messages/cpp)

include_directories (${NODE_DIR}/src
                     ${Boost_INCLUDE_DIRS}
                     ${gtest_SOURCE_DIR}/include
                     ${INCLUDES}
                     )
                     
set(DEFS ${DEFS} -DBOOST_ALL_DYN_LINK)
add_definitions (${DEFS})

file(GLOB TEST_SRCS src/*.h src/*.cpp)

add_executable (Test ${TEST_SRCS})
target_link_libraries (Test
                       sopmq-nodelib
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${LIBS}
                        gtest_main
                       )
