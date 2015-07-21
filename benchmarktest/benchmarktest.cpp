#include <iostream>
#include <benchmark.hpp>
#include <unit.hpp>

void fun() {
	BENCH(fun);
	for(int i = 0; i < 2<<9; ++i) {
		for(int j = 1; j < i; ++j) {
			if(i % j == 0) {
				//std::cout<<j<<std::endl;
			}
		}
	}
}

UNITTEST(benchmarktest) {
	BENCH(main);
	for(int i = 0; i < 2<<10; ++i) {
		fun();
	}

	sweet::Bench b;
	b.stop();

	sweet::Benchmark::printResults();
}
