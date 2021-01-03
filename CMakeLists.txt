cmake_minimum_required(VERSION 3.18)

project(
    IPC_CORE
    VERSION 1.0
    DESCRIPTION "IPC CORE API PROJECT"
    LANGUAGES C CXX
    )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(libipc_NEW)
add_subdirectory(client_api_NEW)
add_subdirectory(client_api_OLD)
add_subdirectory(client_NEW)


#add_subdirectory(msnclient_NEW)
