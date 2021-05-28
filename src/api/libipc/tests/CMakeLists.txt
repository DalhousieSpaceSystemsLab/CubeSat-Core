cmake_minimum_required(VERSION 3.16)
if(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(BACKUP_CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif(CMAKE_RUNTIME_OUTPUT_DIRECTORY)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

foreach(lang ${${LIB}_languages})
    if(${lang} STREQUAL CXX)
        file(GLOB_RECURSE ${LIB}_${lang}_test_sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
    elseif(${lang} STREQUAL C)
        file(GLOB_RECURSE ${LIB}_${lang}_test_sources "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
    else()
    # language not supported
    endif()

    foreach(src ${${LIB}_${lang}_test_sources})
        get_filename_component(test_suffix ${src} NAME_WLE)
        set(test_target "${LIB}_${test_suffix}")
        if(NOT TARGET ${test_target})
            add_executable(${test_target})
            target_sources(${test_target} PRIVATE ${src})
            
            if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
            target_compile_options(${test_target} PRIVATE "-Wall")
            target_compile_options(${test_target} PRIVATE "-Wextra")
            target_compile_options(${test_target} PRIVATE "-Wshadow")
            endif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
            
            target_link_libraries(${test_target} PRIVATE ${LIB})
            target_link_libraries(${test_target} PRIVATE "pthread")
            target_link_libraries(${test_target} PRIVATE "check")
            add_test(
                NAME ${test_target}
                COMMAND valgrind ${CMAKE_CURRENT_BINARY_DIR}/${test_target}${CMAKE_EXECUTABLE_SUFFIX}
                --build-generator "${CMAKE_GENERATOR}"
                --test-command "${CMAKE_CTEST_COMMAND}"
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            ) 
        endif(NOT TARGET ${test_target})
    endforeach(src ${${LIB}_${lang}_test_sources})
    
endforeach(lang ${${LIB}_languages})

if(BACKUP_CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BACKUP_CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif(BACKUP_CMAKE_RUNTIME_OUTPUT_DIRECTORY)