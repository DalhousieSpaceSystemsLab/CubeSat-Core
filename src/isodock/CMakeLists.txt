cmake_minimum_required(VERSION 3.18)


################################################################################
#  OPTIONS GO HERE
################################################################################
option(BUILD_TESTING "[ON/OFF] Build tests in addition to library" OFF)
option(BUILD_EXAMPLES "[ON/OFF] Build examples in addition to library" ON)


################################################################################
#  PROJECT INIT
################################################################################
project(
    ISODOCK
    VERSION 1.0
    DESCRIPTION "ISODOCK"
    LANGUAGES C CXX
)
set(EXE "${PROJECT_NAME}") # this is PROJECT_NAME, NOT CMAKE_PROJECT_NAME
message("CONFIGURING TARGET : ${EXE}")

if(TARGET ${EXE})
    message(FATAL_ERROR "Target ${EXE} already exists in this project!")
else()
    add_executable(${EXE})
endif(TARGET ${EXE})






################################################################################
#  BUILD TYPE CHECK
################################################################################
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



if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    target_compile_options(${EXE} PUBLIC "-Wall")
    target_compile_options(${EXE} PUBLIC "-Wextra")
    target_compile_options(${EXE} PUBLIC "-Wshadow")
    set(BUILD_TESTING ON CACHE BOOL "" FORCE)
    set(BUILD_EXAMPLES ON CACHE BOOL "" FORCE)
endif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)


################################################################################
# DETECT SOURCES RECURSIVELY FROM src FOLDER AND ADD TO BUILD TARGET
################################################################################

get_property(${EXE}_languages GLOBAL PROPERTY ENABLED_LANGUAGES)
foreach(lang ${${EXE}_languages})
    if(${lang} STREQUAL CXX)
        file(GLOB_RECURSE ${EXE}_${lang}_sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
    elseif(${lang} STREQUAL C)
        file(GLOB_RECURSE ${EXE}_${lang}_sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
    elseif(${lang} STREQUAL ASM)
        file(GLOB_RECURSE ${EXE}_${lang}_sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.asm")
    else()
        # language not supported
    endif()
    target_sources(${EXE} PRIVATE ${${EXE}_${lang}_sources})
endforeach(lang ${${EXE}_languages})



################################################################################
# DETECT PRIVATE HEADERS RECURSIVELY FROM src FOLDER
################################################################################
foreach(lang ${${EXE}_languages})
    if(${lang} STREQUAL CXX)
        file(GLOB_RECURSE ${EXE}_${lang}_private_headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
    elseif(${lang} STREQUAL C)
        file(GLOB_RECURSE ${EXE}_${lang}_private_headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
    elseif(${lang} STREQUAL ASM)
    
    else()
        # language not supported
    endif()

    set(${EXE}_${lang}_private_include_directories "")
    foreach(hdr ${${EXE}_${lang}_private_headers})
        get_filename_component(hdr_dir ${hdr} DIRECTORY)
        list(APPEND ${EXE}_${lang}_private_include_directories ${hdr_dir})
    endforeach(hdr ${${EXE}_${lang}_private_headers})
    list(REMOVE_DUPLICATES ${EXE}_${lang}_private_include_directories)
    target_include_directories(${EXE} PRIVATE ${${EXE}_${lang}_private_include_directories})
endforeach(lang ${${EXE}_languages})

################################################################################
# DETECT PUBLIC HEADERS RECURSIVELY FROM inc FOLDER
################################################################################
foreach(lang ${${EXE}_languages})
    if(lang STREQUAL CXX)
        file(GLOB_RECURSE ${EXE}_${lang}_public_headers "${CMAKE_CURRENT_SOURCE_DIR}/inc/*.hpp")
    elseif(lang STREQUAL C)
        file(GLOB_RECURSE ${EXE}_${lang}_public_headers "${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h")
    elseif(lang STREQUAL ASM)
    
    else()
        # language not supported
    endif()

    set(${EXE}_${lang}_public_include_directories "")
    foreach(hdr ${${EXE}_${lang}_public_headers})
        get_filename_component(hdr_dir ${hdr} DIRECTORY)
        list(APPEND ${EXE}_${lang}_public_include_directories ${hdr_dir})
    endforeach(hdr ${${EXE}_${lang}_public_headers})
    list(REMOVE_DUPLICATES ${EXE}_${lang}_public_include_directories)
    target_include_directories(${EXE} PUBLIC ${${EXE}_${lang}_public_include_directories})
endforeach(lang ${${EXE}_languages})

################################################################################
# TEST CONFIGURATION
################################################################################
if(BUILD_TESTING)
    enable_testing()
    include(CTest)
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests")
        add_subdirectory(tests)
    elseif(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test")
        add_subdirectory(test)
    endif()
endif(BUILD_TESTING)


################################################################################
# EXAMPLE CONFIGURATION
################################################################################
if(BUILD_EXAMPLES)
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/examples")
        add_subdirectory(examples)
    elseif(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/example")
        add_subdirectory(example)
    endif()
endif(BUILD_EXAMPLES)


################################################################################
# LINK AGAINST THE NECESSARY TARGETS
################################################################################
target_link_libraries(${EXE} PUBLIC LIB_IPC)
target_link_libraries(${EXE} PUBLIC CLIENT_API)
target_link_libraries(${EXE} PUBLIC MODULES)
target_compile_options(${EXE} PUBLIC "-g")

######################################
# EXAMPLE OF LINKINAG AGAINST PTHREAD
######################################

#   find_package(Threads REQUIRED)
#   if(Threads_FOUND) 
#       if(WIN32)
#           if(CMAKE_USE_WIN32_THREADS_INIT)
#               target_link_libraries(${EXE} PUBLIC pthread)
#               target_compile_options(${EXE} PUBLIC "-lpthread")
#           else()
#               message(FATAL_ERROR "BLAHHH ")
#           endif()
#       elseif(UNIX)
#           if(CMAKE_USE_PTHREADS_INIT)
#               target_link_libraries(${EXE} PUBLIC pthread)
#               target_compile_options(${EXE} PUBLIC "-lpthread")
#           else()
#               message(FATAL_ERROR "BLAHHH ")
#           endif()
#       endif()
#   else()
#       message(FATAL_ERROR "No threading libraries have been found. Aborting!")
#   endif()














