all:
	g++ -Wall --std=c++11 test.cpp -o test --std=gnu++11 -pedantic -lpthread -O0
	./test
#g++ -Wall --std=c++11 algotest.cpp -o algotest -ggdb -O3 --std=gnu++11 -pedantic -lpthread
#	./algotest
