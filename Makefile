all:
	g++ -Wall --std=c++11 optiontest.cpp -o optiontest --std=gnu++11 -ggdb \
	-pedantic
	./optiontest --test 1 -b true -c 3 -c 4
