cmake_minimum_required(VERSION 2.8.10)

## we have some extra modules here
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

## thats our project
project(dpp)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../..)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

##############################################################################

## our headers
set(dpp_HEADERS 
	ast.hpp  
	error.hpp  
	lexer.hpp  
	location.hpp  
	numberconv.hpp
	parser.hpp  
	token.hpp
	vistor.hpp
	dotvistor.hpp
	outvistor.hpp
)

## well and the sources
set(dpp_SOURCES 
	ast.cpp  
	error.cpp  
	lexer.cpp  
	location.cpp  
	main.cpp  
	numberconv.cpp
	parser.cpp  
	token.cpp
	dotvistor.cpp
	outvistor.cpp
)


## we love c++0x and a chatty compiler that comments on our code
set(CMAKE_CXX_FLAGS "-Wall -Wextra -ggdb --std=c++0x -pedantic")

include_directories(
	${CMAKE_CURRENT_BINARY_DIR} 
)

add_executable(dpp
    ${dpp_SOURCES}
	${dpp_HEADERS}
)
