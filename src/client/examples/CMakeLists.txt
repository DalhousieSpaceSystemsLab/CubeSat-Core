cmake_minimum_required(VERSION 3.16)
if(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(BACKUP_CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif(CMAKE_RUNTIME_OUTPUT_DIRECTORY)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

foreach(lang ${${EXE}_languages})
    if(${lang} STREQUAL CXX)
        file(GLOB_RECURSE ${EXE}_${lang}_example_sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
    elseif(${lang} STREQUAL C)
        file(GLOB_RECURSE ${EXE}_${lang}_example_sources "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
    else()
        # language not supported
    endif()

    foreach(src ${${EXE}_${lang}_example_sources})
        get_filename_component(example_suffix ${src} NAME_WLE)
        set(example_target "${EXE}_${example_suffix}")
        if(NOT TARGET ${example_target})
            add_executable(${example_target})
            target_sources(${example_target} PRIVATE ${src})
            target_link_libraries(${example_target} PRIVATE ${EXE})
        endif(NOT TARGET ${example_target})
    endforeach(src ${${EXE}_${lang}_example_sources})
    
endforeach(lang ${${EXE}_languages})


if(BACKUP_CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BACKUP_CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif(BACKUP_CMAKE_RUNTIME_OUTPUT_DIRECTORY)
