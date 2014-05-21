cmake_minimum_required(VERSION 2.8.10)

## we have some extra modules here
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

## thats our project
project(recde)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

##############################################################################

## our headers
set(recde_HEADERS 
	expr.hpp  
	recdec.hpp  
	ruleparser.hpp  
	rulepart.hpp  
	rulestore.hpp
	token.hpp
	output.hpp
)

## well and the sources
set(recde_SOURCES 
	expr.cpp  
	main.cpp  
	recdec.cpp  
	ruleparser.cpp  
	rulepart.cpp
	rulestore.cpp  
	token.cpp
	output.cpp
)


## we love c++0x and a chatty compiler that comments on our code
set(CMAKE_CXX_FLAGS "-Wall -Wextra -ggdb --std=c++11 -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=auto")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

include_directories(
	${CMAKE_CURRENT_BINARY_DIR} 
	${CMAKE_CURRENT_SOURCE_DIR}/includes
)

add_executable(recde
    ${recde_SOURCES}
	${recde_HEADERS}
)
