	#g++ -Wall --std=c++11 test.cpp -o test --std=gnu++11 -pedantic -lpthread -O0
	#./test
#g++ -Wall --std=c++11 algotest.cpp -o algotest -ggdb -O3 --std=gnu++11 -pedantic -lpthread
#	./algotest
#g++ -Wall --std=c++11 convtest.cpp -o convtest -ggdb -O3 --std=gnu++11 -pedantic -lpthread
#	./convtest
#	g++ -Wall --std=c++11 optiontest.cpp -o optiontest --std=gnu++11 -ggdb \
#	-pedantic
#	./optiontest --test 1 -b true -c 3 -f 0 --funs 1 -f 2 -h

#all:
#	./makedb.sh
#	g++ -Wall --std=c++11 -lsqlite3 -lboost_regex sweetqltest.cpp -o qltest \
#	--std=c++11 -O1 -ggdb
#	./qltest

all:
	g++ -Wall --std=c++11 sjsontest.cpp -o sjsontest -ggdb
	./sjsontest
