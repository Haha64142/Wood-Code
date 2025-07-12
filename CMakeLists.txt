cmake_minimum_required(VERSION 3.10)

project(WoodCode)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(woodcode_lib src/woodcode.cpp)
target_include_directories(woodcode_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# CLI executable
add_executable("woodcode" src/cli.cpp)
target_link_libraries("woodcode" PRIVATE woodcode_lib)

# Main executable (Non-CLI, less powerful)
add_executable("WoodCodeApp" src/main.cpp)
target_link_libraries("WoodCodeApp" PRIVATE woodcode_lib)