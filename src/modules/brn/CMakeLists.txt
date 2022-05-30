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
    MODULE_BRN
    VERSION 1.0
    DESCRIPTION "BURNWIRE MODULE FOR LORIS"
    LANGUAGES C CXX
)
set(LIB "${PROJECT_NAME}") # this is PROJECT_NAME, NOT CMAKE_PROJECT_NAME
message("CONFIGURING TARGET : ${LIB}")

if(TARGET ${LIB})
    message(FATAL_ERROR "Target ${LIB} already exists in this project!")
else()
    add_library(${LIB})
endif(TARGET ${LIB})






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
    target_compile_options(${LIB} PUBLIC "-Wall")
    target_compile_options(${LIB} PUBLIC "-Wextra")
    target_compile_options(${LIB} PUBLIC "-Wshadow")
    set(BUILD_TESTING ON CACHE BOOL "" FORCE)
    set(BUILD_EXAMPLES ON CACHE BOOL "" FORCE)
endif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)


################################################################################
# DETECT SOURCES RECURSIVELY FROM src FOLDER AND ADD TO BUILD TARGET
################################################################################

get_property(${LIB}_languages GLOBAL PROPERTY ENABLED_LANGUAGES)
foreach(lang ${${LIB}_languages})
    if(${lang} STREQUAL CXX)
        file(GLOB_RECURSE ${LIB}_${lang}_sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
    elseif(${lang} STREQUAL C)
        file(GLOB_RECURSE ${LIB}_${lang}_sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
    elseif(${lang} STREQUAL ASM)
        file(GLOB_RECURSE ${LIB}_${lang}_sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.asm")
    else()
        # language not supported
    endif()
    target_sources(${LIB} PRIVATE ${${LIB}_${lang}_sources})
endforeach(lang ${${LIB}_languages})



################################################################################
# DETECT PRIVATE HEADERS RECURSIVELY FROM src FOLDER
################################################################################
foreach(lang ${${LIB}_languages})
    if(${lang} STREQUAL CXX)
        file(GLOB_RECURSE ${LIB}_${lang}_private_headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
    elseif(${lang} STREQUAL C)
        file(GLOB_RECURSE ${LIB}_${lang}_private_headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
    elseif(${lang} STREQUAL ASM)
    
    else()
        # language not supported
    endif()

    set(${LIB}_${lang}_private_include_directories "")
    foreach(hdr ${${LIB}_${lang}_private_headers})
        get_filename_component(hdr_dir ${hdr} DIRECTORY)
        list(APPEND ${LIB}_${lang}_private_include_directories ${hdr_dir})
    endforeach(hdr ${${LIB}_${lang}_private_headers})
    list(REMOVE_DUPLICATES ${LIB}_${lang}_private_include_directories)
    target_include_directories(${LIB} PRIVATE ${${LIB}_${lang}_private_include_directories})
endforeach(lang ${${LIB}_languages})

################################################################################
# DETECT PUBLIC HEADERS RECURSIVELY FROM inc FOLDER
################################################################################
foreach(lang ${${LIB}_languages})
    if(lang STREQUAL CXX)
        file(GLOB_RECURSE ${LIB}_${lang}_public_headers "${CMAKE_CURRENT_SOURCE_DIR}/inc/*.hpp")
    elseif(lang STREQUAL C)
        file(GLOB_RECURSE ${LIB}_${lang}_public_headers "${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h")
    elseif(lang STREQUAL ASM)
    
    else()
        # language not supported
    endif()

    set(${LIB}_${lang}_public_include_directories "")
    foreach(hdr ${${LIB}_${lang}_public_headers})
        get_filename_component(hdr_dir ${hdr} DIRECTORY)
        list(APPEND ${LIB}_${lang}_public_include_directories ${hdr_dir})
    endforeach(hdr ${${LIB}_${lang}_public_headers})
    list(REMOVE_DUPLICATES ${LIB}_${lang}_public_include_directories)
    target_include_directories(${LIB} PUBLIC ${${LIB}_${lang}_public_include_directories})
endforeach(lang ${${LIB}_languages})


################################################################################
# SPECIAL AND PROJECT SPECIFIC OPTIONS
################################################################################

target_compile_options(${LIB} PUBLIC "-g")



################################################################################
# LINK AGAINST THE NECESSARY LIBRARIES 
################################################################################
target_link_libraries(${LIB} PUBLIC LIB_IPC)
target_link_libraries(${LIB} PUBLIC CLIENT_API)

#Link against pthread
find_package(Threads REQUIRED)
if(Threads_FOUND) 
    if(WIN32)
        if(CMAKE_USE_WIN32_THREADS_INIT)
            target_link_libraries(${LIB} PUBLIC pthread)
            target_compile_options(${LIB} PUBLIC "-lpthread")
        else()
            message(FATAL_ERROR "BLAHHH ")
        endif()
    elseif(UNIX)
        if(CMAKE_USE_PTHREADS_INIT)
            target_link_libraries(${LIB} PUBLIC pthread)
            target_compile_options(${LIB} PUBLIC "-lpthread")
        else()
            message(FATAL_ERROR "BLAHHH ")
        endif()
    endif()
else()
    message(FATAL_ERROR "No threading libraries have been found. Aborting!")
endif()

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
