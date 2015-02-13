all: stringcmptest.cpp ../stringcmp.hpp
	clang++ -Wall --std=c++11 stringcmptest.cpp -o stringcmptest -pedantic \
	-lpthread -I.. -Wextra -ggdb -march=native -O3
	./stringcmptest

#-lpthread -Ofast -march=native -I.. -Wextra -ggdb
