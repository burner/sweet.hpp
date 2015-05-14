#CXX=clang++

test: all
	./amber -i amber2.am -n amber2
#./amber amberhead.am	

all:
	${CXX} --std=c++14 -Wall -Wextra amber.cpp output.cpp -o amber -ggdb -lboost_program_options
