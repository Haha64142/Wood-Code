cmake_minimum_required(VERSION 3.10)

project(WoodCode)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPACK_PACKAGE_NAME "WoodCode")
set(CPACK_PACKAGE_VERSION "1.3.5")
set(CPACK_GENERATOR "ZIP;TGZ")
include(CPack)

add_library(woodcode_lib src/woodcode.cpp)
target_include_directories(woodcode_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# CLI executable
add_executable("woodcode" src/cli.cpp)
target_link_libraries("woodcode" PRIVATE woodcode_lib)

install(TARGETS woodcode)

# Main executable (Non-CLI, less powerful)
add_executable("WoodCodeApp" src/main.cpp)
target_link_libraries("WoodCodeApp" PRIVATE woodcode_lib)

install(TARGETS WoodCodeApp)