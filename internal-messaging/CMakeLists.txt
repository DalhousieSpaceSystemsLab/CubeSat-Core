cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_STANDARD 14)
set(GCC_COVERAGE_COMPILE_FLAGS "-I ${CMAKE_CURRENT_LIST_DIR}/")

# add all .cc and .h files in internal-messaging
file(GLOB_RECURSE SRC "*.cc" "*.h")

# remove the other garbage with main.cc files in it
list(REMOVE_ITEM SRC
	"${CMAKE_CURRENT_LIST_DIR}/unixdomain/test/UnixDomainServer_test.cc"
	"${CMAKE_CURRENT_LIST_DIR}/unixdomain/test/UnixDomainClient_test.cc")

message(STATUS "SRC = ${SRC}")

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(
	include
	${CMAKE_CURRENT_LIST_DIR}/Default
	${CMAKE_CURRENT_LIST_DIR}/message/header
	${CMAKE_CURRENT_LIST_DIR}/message/identifiers
	${CMAKE_CURRENT_LIST_DIR}/messaging-service/header
	${CMAKE_CURRENT_LIST_DIR}/messenger/header
	${CMAKE_CURRENT_LIST_DIR}/unixdomain/header)

# add internal-messaging
add_library(internal-messaging ${SRC})

# Link everything
add_executable(message_test ${CMAKE_CURRENT_LIST_DIR}/message/test/message_test.cc ${SRC})
target_link_libraries(message_test ${GTEST_LIBRARIES} internal-messaging pthread)