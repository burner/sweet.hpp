#CXX=clang++

all:
	${CXX} --std=c++14 -I.. -Wall -Wextra amber.cpp output.cpp -o amber -ggdb

test: all
	./amber -i amber3.am -n amber3 -o amber3.hpp
	g++ amber2.cpp --std=c++11 -o amber3 

