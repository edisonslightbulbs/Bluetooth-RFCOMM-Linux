cmake_minimum_required(VERSION 3.11)
project(android-linux-bluetooth-rfcomm)

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

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# setup target
add_executable(android-linux-bluetooth-rfcomm
    ${PROJECT_DIR}/bluetooth.c
    )

# link bluetooth dependencies
set(EXTRA_LIBS ${YARP_LIBRARIES})
list(APPEND EXTRA_LIBS "bluetooth")
target_link_libraries(android-linux-bluetooth-rfcomm ${EXTRA_LIBS})

# execute target
option(EXECUTE_TARGET "Execute Project Target" ON)            # default OFF
if(EXECUTE_TARGET)
    add_custom_command(TARGET android-linux-bluetooth-rfcomm
        POST_BUILD
        COMMAND ./build/bin/android-linux-bluetooth-rfcomm
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
endif()
