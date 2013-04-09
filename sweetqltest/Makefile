all:
	./makedb.sh
	g++ -Wall --std=c++11 -lsqlite3 -lboost_regex sweetqltest.cpp -o qltest \
	--std=c++11 -O1 -ggdb -I..
	./qltest
