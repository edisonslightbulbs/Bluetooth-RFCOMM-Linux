cmake_minimum_required(VERSION 3.11)
project(bluez)

# optimization for dev environments
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# project version
set(MAJOR 0)
set(MINOR 1)
set(REMAP_VERSION ${MAJOR}.${MINOR})

# flags
set(CMAKE_CXX_FLAGS "-pthread")

# set cmake modules path
list(APPEND CMAKE_MODULE_PATH "$ENV{CMAKE_MODULE_PATH}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

option(BUILD_SHARED_LIBS "Build Shared Library" ON)           # default ON

# set path variables
set(PROJECT_DIR ${CMAKE_CURRENT_LIST_DIR})

set(BIN_DIR ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR})

# tidy project
option(TIDY_PROJECT "Tidy up and optimize code base" ON)   # default OFF
if(TIDY_PROJECT)
    if(UNIX AND NOT APPLE)
        set(CMAKE_CXX_CLANG_TIDY
            clang-tidy -p pwd
            --checks=-*,readability-*,clang-analyzer-*,performance-*)
    endif()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

# if(NOT CMAKE_BUILD_TYPE)
#     set(CMAKE_BUILD_TYPE Release)
# endif()


# setup target
add_executable(bluez
    ${PROJECT_DIR}/bluetooth.c
    )

# execute target
option(EXECUTE_TARGET "Execute Project Target" OFF)            # default OFF
if(EXECUTE_TARGET)
    add_custom_command(TARGET bluez
        POST_BUILD
        COMMAND ./bluetooth
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
endif()
