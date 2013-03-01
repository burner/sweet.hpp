all:
	#g++ -Wall --std=c++11 optiontest.cpp -o optiontest --std=gnu++11 -ggdb \
	#-pedantic
	#./optiontest --test 1 -b true -c 3 -c 4
	g++ -Wall --std=c++11 test.cpp -o test --std=gnu++11 -pedantic -lpthread -O0
	./test
#g++ -Wall --std=c++11 algotest.cpp -o algotest -ggdb -O3 --std=gnu++11 -pedantic -lpthread
#	./algotest
