cmake_minimum_required(VERSION 2.8.7)

## we have some extra modules here
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

## thats our project
project(amber)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

set(amber_SOURCES 
amber.cpp
output.cpp
)

set(amber_HEADERS
amber.hpp
)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -ggdb --std=c++1y -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

add_executable(amber
    ${amber_SOURCES}
	${amber_HEADERS}
)
