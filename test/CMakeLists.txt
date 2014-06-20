find_package(Boost COMPONENTS system program_options log thread REQUIRED)

find_package(Protobuf REQUIRED)
set(INCLUDES ${INCLUDES} ${PROTOBUF_INCLUDE_DIR})
set(LIBS ${LIBS} ${PROTOBUF_LIBRARIES})

set(INCLUDES ${INCLUDES} ../messages/cpp)
set(INCLUDES ${INCLUDES} ${SHARED_DIR}/src)
set(INCLUDES ${INCLUDES} ${NODE_DIR}/src)
set(INCLUDES ${INCLUDES} ${CLIENT_DIR}/src)

include_directories (${Boost_INCLUDE_DIRS}
                     ${gtest_SOURCE_DIR}/include
                     ${INCLUDES}
                     )
                     
set(DEFS ${DEFS} -DBOOST_ALL_DYN_LINK)
add_definitions (${DEFS})

file(GLOB TEST_SRCS src/*.h src/*.cpp)

add_executable (Test ${TEST_SRCS})
target_link_libraries (Test
                       sopmq-nodelib
                       sopmq-client
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${LIBS}
                        gtest_main
                       )
