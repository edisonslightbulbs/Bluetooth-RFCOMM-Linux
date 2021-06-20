cmake_minimum_required(VERSION 3.11)
project(rfcomm)

# optimization for dev environments
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# project version
set(MAJOR 0)
set(MINOR 1)
set(REMAP_VERSION ${MAJOR}.${MINOR})

# flags
set(CMAKE_CXX_FLAGS "-pthread")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# set cmake modules path
list(APPEND CMAKE_MODULE_PATH "$ENV{CMAKE_MODULE_PATH}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# link bluetooth dependencies
set(EXTRA_LIBS ${YARP_LIBRARIES})
list(APPEND EXTRA_LIBS "bluetooth")

# set path variables
set(PROJECT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(BIN_DIR ${PROJECT_BINARY_DIR}/bin)
set(SRC_DIR ${PROJECT_DIR}/src)
set(INCLUDE_DIR ${PROJECT_DIR}/include)

option(BUILD_SHARED_LIBS "Build Shared Library" ON)           # default ON
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR})

# find header files
set (INCLUDE_DIRS "")
file(GLOB_RECURSE HEADERS
    ${INCLUDE_DIR}/*.h
        ${INCLUDE_DIR}/*.hpp
    )
foreach (HEADER ${HEADERS})
    get_filename_component(DIR ${HEADER} PATH)
    list (APPEND INCLUDE_DIRS ${DIR})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

# find source files
file(GLOB_RECURSE SRC_DIR
    ${SRC_DIR}/*.c
    )

# setup target
add_executable(rfcomm
    ${PROJECT_DIR}/main.c
    ${SRC_DIR}
    )

# include directories
target_include_directories(rfcomm PRIVATE
    ${INCLUDE_DIRS}
    )

# link bluetooth dependencies
target_link_libraries(rfcomm ${EXTRA_LIBS})

# execute target
option(EXECUTE_TARGET "Execute Project Target" ON)            # default OFF
if(EXECUTE_TARGET)
    add_custom_command(TARGET rfcomm
        POST_BUILD
        COMMAND ./build/bin/rfcomm
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
endif()
