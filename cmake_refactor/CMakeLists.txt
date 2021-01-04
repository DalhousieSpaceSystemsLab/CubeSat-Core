cmake_minimum_required(VERSION 3.18)

option(BUILD_TESTING "[ON/OFF] Option to build tests" OFF)
option(CMAKE_VERBOSE_MAKEFILE "[ON/OFF] Option to emit makefile commands to stdout" OFF)

project(
    IPC_CORE
    VERSION 1.0
    DESCRIPTION "IPC CORE API PROJECT"
    LANGUAGES C CXX
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(BUILD_TESTING ON CACHE BOOL "OPTION TO BUILD TESTING" FORCE)
endif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

set(SUPPORTED_BUILD_TYPES "")
list(APPEND SUPPORTED_BUILD_TYPES "Debug")
list(APPEND SUPPORTED_BUILD_TYPES "Release")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${SUPPORTED_BUILD_TYPES})
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type chosen by the user at configure time")
else()
    if(NOT CMAKE_BUILD_TYPE IN_LIST SUPPORTED_BUILD_TYPES)
        message("Build type : ${CMAKE_BUILD_TYPE} is not a supported build type.")
        message("Supported build types are:")
        foreach(type ${SUPPORTED_BUILD_TYPES})
            message("- ${type}")
        endforeach(type ${SUPPORTED_BUILD_TYPES})
        message(FATAL_ERROR "The configuration script will now exit.")
    endif(NOT CMAKE_BUILD_TYPE IN_LIST SUPPORTED_BUILD_TYPES)
endif(NOT CMAKE_BUILD_TYPE)


if(WIN32)
    set(CMAKE_EXECUTABLE_SUFFIX ".exe")
elseif(UNIX AND NOT APPLE)
    set(CMAKE_EXECUTABLE_SUFFIX ".out")
elseif(APPLE)
    set(CMAKE_EXECUTABLE_SUFFIX ".out")
else()
    message(FATAL_ERROR "[ERROR] platform: ${CMAKE_HOST_SYSTEM_NAME} not supported")

endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} CACHE INTERNAL "")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} CACHE INTERNAL "")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} CACHE INTERNAL "")


execute_process(
    COMMAND "git" "submodule" "update" "--remote" "--recursive"
    RESULT_VARIABLE GIT_SUBMODULE_UPDATE_FAILED
    OUTPUT_VARIABLE GIT_SUBMODULE_UPDATE_OUTPUT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ECHO_OUTPUT_VARIABLE
    )

execute_process(
    COMMAND "sync"
)

if(BUILD_TESTING)
    enable_testing()
    include(CTest)
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
        add_subdirectory(test)
    elseif(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
        add_subdirectory(tests)
    endif()
endif()


set(TOOLCHAIN_PREFIX arm-none-eabi)
set(CMAKE_C_COMPILER_NAME gcc)
set(CMAKE_ASM_COMPILER_NAME gcc)
set(CMAKE_CXX_COMPILER_NAME g++)
set(CMAKE_OBJCOPY_NAME objcopy)
set(CMAKE_OBJDUMP_NAME objdump)
set(CMAKE_SIZE_NAME size)
set(CMAKE_GDB_NAME gdb) #might also have gdb.py extension

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

find_program(CMAKE_C_COMPILER ${CMAKE_C_COMPILER_NAME} REQUIRED)
find_program(CMAKE_ASM_COMPILER ${CMAKE_ASM_COMPILER_NAME} REQUIRED)
find_program(CMAKE_CXX_COMPILER ${CMAKE_CXX_COMPILER_NAME} REQUIRED)
find_program(CMAKE_OBJCOPY ${CMAKE_OBJCOPY_NAME} REQUIRED)
find_program(CMAKE_OBJDUMP ${CMAKE_OBJDUMP_NAME} REQUIRED)
find_program(CMAKE_SIZE ${CMAKE_SIZE_NAME} REQUIRED)


if(NOT COMMAND _add_executable)
function(add_executable executable)
    _add_executable(${executable} ${ARGN})

    # for now, just exiting because I can't get this to work consistently across platforms
    return()


    add_custom_target(${executable}_postbuild ALL DEPENDS ${executable})
    add_custom_command( 
        TARGET ${executable}_postbuild
        POST_BUILD
        DEPENDS ${executable}
        COMMENT "Built executable ${executable} with the following size:"
        COMMAND ${CMAKE_SIZE} -B "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${executable}$CACHE{CMAKE_EXECUTABLE_SUFFIX}"
    )

    foreach(ext "ihex;binary;")
        add_custom_command(
            TARGET ${executable}_postbuild
            POST_BUILD
            DEPENDS ${executable}
            COMMAND ${CMAKE_OBJCOPY} -O ${ext} -I elf32-little "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${executable}$CACHE{CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/metadata/${executable}/${exectuable}.${ext}"
        )
    endforeach(ext "ihex;binary;")

    add_custom_command( 
        TARGET ${executable}_postbuild
        POST_BUILD
        DEPENDS ${executable}
        COMMENT "Generating lss file from ${executable} using ${CMAKE_OBJDUMP}"
        COMMAND ${CMAKE_OBJDUMP} -xh "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${executable}$CACHE{CMAKE_EXECUTABLE_SUFFIX}" > "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/metadata/${executable}/${executable}.lss"
    )
endfunction(add_executable executable)
endif(NOT COMMAND _add_executable)



if(NOT COMMAND _add_library)
function(add_library library)
    _add_library(${library} ${ARGN})

    # for now, just exiting because I can't get this to work consistently across platforms
    return()

    add_custom_target(${library}_postbuild ALL DEPENDS ${library})

    add_custom_command( 
        TARGET ${library}_postbuild
        POST_BUILD
        DEPENDS ${library}
        COMMENT "Built library ${library} with the following size:"
        COMMAND ${CMAKE_SIZE} -B "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}   ${library}"
    )

    foreach(ext "ihex;binary;")
        add_custom_command(
            TARGET ${library}_postbuild
            POST_BUILD
            DEPENDS ${library}
            COMMAND ${CMAKE_OBJCOPY} -O ${ext} -I elf32-little "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${library}" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/metadata/${library}/${library}.${ext}"
        )
    endforeach(ext "ihex;binary;")

    add_custom_command( 
        TARGET ${library}_postbuild
        POST_BUILD
        DEPENDS ${library}
        COMMENT "Generating lss file from ${library} using ${CMAKE_OBJDUMP}"
        COMMAND ${CMAKE_OBJDUMP} -xh "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${library}" > "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/metadata/${library}/${library}.lss"
    )
endfunction(add_library library)
endif(NOT COMMAND _add_library)

# dependencies come first
add_subdirectory(api/libipc)
add_subdirectory(api/client_api)

# executable targets after, and link against necessary dependencies
add_subdirectory(ipcd)
add_subdirectory(subsys_server)
add_subdirectory(client)
#add_subdirectory(msnclient) # can't get this damn thing to build... SO MANY ERRORS IN THE SOURCE CODE...

