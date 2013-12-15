all:
	clang++ -Wall -Wextra --std=c++11 fixedtest.cpp -o fixedtest -pedantic\
	 -O0 -I.. -ggdb -stdlib=libc++ -lc++abi -nodefaultlibs -lc++ -lc++abi -lm -lc -lgcc_s
	./fixedtest
