cmake_minimum_required(VERSION 3.10)

project(WoodCode)

set(CMAKE_CXX_STANDARD 17)

set(CPACK_PACKAGE_NAME "WoodCode")
set(CPACK_PACKAGE_VERSION "1.3.5")
set(CPACK_GENERATOR "ZIP;TGZ")
include(CPack)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(WoodCode ${SOURCES})

install(TARGETS WoodCode)
