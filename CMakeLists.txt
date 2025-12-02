cmake_minimum_required(VERSION 3.12)

project(WoodCode)

if(MSVC)
    # Can't use the default cmake c++20 version setting
    if(MSVC_LEGACY)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")
    endif()

else()
    set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CPACK_PACKAGE_NAME "WoodCode")
set(CPACK_PACKAGE_VERSION "1.4.0")
set(CPACK_GENERATOR "ZIP;TGZ")
include(CPack)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# Verify build type
set(SUPPORTED_BUILD_TYPES Debug Release RelWithDebInfo MinSizeRel)

if(NOT CMAKE_BUILD_TYPE IN_LIST SUPPORTED_BUILD_TYPES)
    message(FATAL_ERROR "\nInvalid build type: ${CMAKE_BUILD_TYPE}\nChoose one of: ${SUPPORTED_BUILD_TYPES}")
endif()

add_library(woodcode_lib src/woodcode.cpp)
target_include_directories(woodcode_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# CLI executable
add_executable("woodcode" src/cli.cpp)

# Main executable (Non-CLI, less powerful)
add_executable("WoodCodeApp" src/main.cpp)

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
endif()

foreach(target woodcode WoodCodeApp)
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    install(TARGETS ${target} RUNTIME DESTINATION bin)
    target_link_libraries(${target} PRIVATE woodcode_lib)
    target_link_options(${target} PRIVATE)
endforeach()

# For single-config generators
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# For multi-config generators
foreach(config DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} ${CMAKE_BINARY_DIR}/bin)
endforeach()

# Get help files
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/help")
file(GLOB HELP_FILES "${PROJECT_SOURCE_DIR}/help/*.help")

install(FILES ${HELP_FILES} DESTINATION help)

# Choose behavior based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Creating links for help files (Debug mode)")

    foreach(help_file ${HELP_FILES})
        get_filename_component(filename ${help_file} NAME)
        set(dest "${CMAKE_BINARY_DIR}/help/${filename}")
        file(CREATE_LINK ${help_file} ${dest})
    endforeach()
else()
    message(STATUS "Copying help files (Non-Debug mode)")

    foreach(help_file ${HELP_FILES})
        get_filename_component(filename ${help_file} NAME)
        configure_file(${help_file}
            "${CMAKE_BINARY_DIR}/help/${filename}"
            COPYONLY)
    endforeach()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    enable_testing()

    add_executable(WoodCodeTests tests/tests.cpp)
    target_link_libraries(WoodCodeTests PRIVATE woodcode_lib)

    add_test(NAME WoodCodeTests COMMAND WoodCodeTests)
endif()