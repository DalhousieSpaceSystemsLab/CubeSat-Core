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

# if(BUILD_TESTING)
#     enable_testing()
#     include(CTest)
#     if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
#         add_subdirectory(test)
#     elseif(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
#         add_subdirectory(tests)
#     endif()
# endif()

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
function(add_executable exe)
    _add_executable(${exe} ${ARGN})

    if(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
        set(meta_dir "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/metadata")
    else()
        set(meta_dir "${CMAKE_CURRENT_BINARY_DIR}/metadata")
    endif(CMAKE_RUNTIME_OUTPUT_DIRECTORY)

    if(NOT EXISTS ${meta_dir})
        file(MAKE_DIRECTORY ${meta_dir})
    endif(NOT EXISTS ${meta_dir})

    if(NOT EXISTS "${meta_dir}/${exe}")
        file(MAKE_DIRECTORY "${meta_dir}/${exe}")
    endif(NOT EXISTS "${meta_dir}/${exe}")

    set(exe_full "${exe}${CMAKE_EXECUTABLE_SUFFIX}")
    set(exe_hex "${exe}.hex")
    set(exe_bin "${exe}.bin")

    add_custom_target(${exe}_postbuild ALL DEPENDS ${exe})
    add_custom_command( 
        TARGET ${exe}_postbuild
        POST_BUILD
        DEPENDS ${exe}
        COMMENT "Built exe ${exe} with the following size:"
        COMMAND ${CMAKE_SIZE} -B "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${exe_full}"
    )

    add_custom_command( 
        TARGET ${exe}_postbuild
        POST_BUILD
        DEPENDS ${exe}
        BYPRODUCTS "${meta_dir}/${exe}/${exe_hex}"
        COMMENT "Producing a hex output using ${CMAKE_OBJCOPY}"
        COMMAND ${CMAKE_OBJCOPY} -O ihex -I elf32-little "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${exe_full}" "${meta_dir}/${exe}/${exe_hex}"
    )

    add_custom_command(
        TARGET ${exe}_postbuild
        POST_BUILD
        DEPENDS ${exe}
        BYPRODUCTS "${meta_dir}/${exe}/${exe_bin}"
        COMMAND ${CMAKE_OBJCOPY} -O binary -I elf32-little "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${exe_full}" "${meta_dir}/${exe}/${exe_bin}"
    )

    add_custom_command( 
        TARGET ${exe}_postbuild
        POST_BUILD
        DEPENDS ${exe}
        COMMENT "Generating lss file from ${exe} using ${CMAKE_OBJDUMP}"
        COMMAND ${CMAKE_OBJDUMP} -xh "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${exe_full}" > "${meta_dir}/${exe}/${exe}.lss"
    )
endfunction(add_executable exe)
endif(NOT COMMAND _add_executable)


if(NOT COMMAND _add_library)
function(add_library lib)
    _add_library(${lib} ${ARGN})

    # for now, just aborting. When namespaced targets are received as 
    # inputs to this function, for some reason a custom command conflict
    # gets created since aparently an arbitrary number of custom targets
    # can exist within the same namespace. Then, when build time occurs,
    # cmake cannot produce a DAG since the dependency order of the 
    # custom targets is unclear 
    return()

    add_custom_target(${lib}_postbuild ALL DEPENDS ${lib})

    add_custom_command( 
        TARGET ${lib}_postbuild
        POST_BUILD
        DEPENDS ${lib}
        COMMENT "Built lib ${lib} with the following size:"
        COMMAND ${CMAKE_SIZE} -B "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}   ${lib}"
    )

    add_custom_command( 
        TARGET ${lib}_postbuild
        POST_BUILD
        DEPENDS ${lib}
        COMMENT "Generating lss file from ${lib} using ${CMAKE_OBJDUMP}"
        BYPRODUCTS "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/metadata/${lib}/${lib}.lss"
        COMMAND ${CMAKE_OBJDUMP} -xh "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${lib}" > "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/metadata/${lib}/${lib}.lss"
    )
endfunction(add_library lib)
endif(NOT COMMAND _add_library)

# dependencies come first
add_subdirectory(api/libipc)
add_subdirectory(api/client_api)
add_subdirectory(subsystem/servers)

# executable targets after, and link against necessary dependencies
add_subdirectory(ipcd)
add_subdirectory(client)
add_subdirectory(subsystem/dock)

# TARGET_COMPILE_OPTIONS(${BIN_TEST}
#   BEFORE PUBLIC "-g"
# )


#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../JTOK)
