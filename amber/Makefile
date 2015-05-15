#CXX=clang++

test: all
	./amber -i amber2.am -n amber2 -o amber2.hpp
	g++ amber2.cpp --std=c++11 -o amber2 

all:
	${CXX} --std=c++14 -Wall -Wextra amber.cpp output.cpp -o amber -ggdb -lboost_program_options
