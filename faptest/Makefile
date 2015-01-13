CFLAGS=-Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused

all: faptest.cpp ../fap.hpp
	clang++ -Wall -Wextra -Wpedantic $(CFLAGS) --std=c++14 faptest.cpp -o faptest -lpthread -I.. -ggdb
	./faptest
