all:
	g++ -Wall --std=c++11 algotest.cpp -o algotest --std=gnu++11 -ggdb -pedantic -lpthread -O3
	./algotest
