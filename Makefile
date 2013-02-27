all:
	g++ -Wall --std=c++11 convtest.cpp -o convtest --std=gnu++11 -pedantic -lpthread -O3
	./convtest
