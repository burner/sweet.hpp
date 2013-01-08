all:
	g++ -Wall --std=c++11 test.cpp -o test --std=gnu++11 -ggdb
	./test
