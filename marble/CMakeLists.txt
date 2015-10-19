cmake_minimum_required(VERSION 2.8.7)

## we have some extra modules here
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

## thats our project
project(marble)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

set(marble_SOURCES 
	src/request.cpp
	src/request_parser.cpp
	src/server_http.cpp
	src/callback.cpp
)

set(marble_HEADERS
	src/client_http.hpp
	src/client_https.hpp
	src/request.hpp
	src/request_parser.hpp
	src/server_http.hpp
	src/server_https.hpp
	src/callback.hpp
	src/function.hpp
	src/callbacktype.hpp
)

set(CMAKE_CXX_FLAGS "-Wall -Wextra --std=c++14 -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

add_library(marble SHARED
    ${marble_SOURCES}
	${marble_HEADERS}
)
