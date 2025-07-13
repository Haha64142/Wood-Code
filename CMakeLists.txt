cmake_minimum_required(VERSION 3.10)

project(WoodCode)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

add_library(woodcode_lib src/woodcode.cpp)
target_include_directories(woodcode_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# CLI executable
add_executable("woodcode" src/cli.cpp)
target_link_libraries("woodcode" PRIVATE woodcode_lib)

# Main executable (Non-CLI, less powerful)
add_executable("WoodCodeApp" src/main.cpp)
target_link_libraries("WoodCodeApp" PRIVATE woodcode_lib)

# Get help files
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/help")
file(GLOB HELP_FILES "${PROJECT_SOURCE_DIR}/help/*.help")

# Choose behavior based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Creating symlinks for help files (Debug mode)")

    foreach(help_file ${HELP_FILES})
        get_filename_component(filename ${help_file} NAME)
        set(dest "${CMAKE_CURRENT_BINARY_DIR}/help/${filename}")
        execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${help_file} ${dest})
    endforeach()
else()
    message(STATUS "Copying help files (Non-Debug mode)")

    foreach(help_file ${HELP_FILES})
        get_filename_component(filename ${help_file} NAME)
        configure_file(${help_file}
            "${CMAKE_CURRENT_BINARY_DIR}/help/${filename}"
            COPYONLY)
    endforeach()
endif()