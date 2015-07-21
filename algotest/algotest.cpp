#include <vector>
#include <iostream>
#include <chrono>
#include <set>
#include <parallelalgo.hpp>
#include <logger.hpp>
#include <benchmark.hpp>
#include <unit.hpp>

using namespace sweet;

void fun(int& a) {
	a *= 5;
}

void fun2(int in, std::insert_iterator<std::set<int>>& iter) {
	 *iter++ = (in) * 127 % 32;
}

int fun2_5(int in) {
	return in * 127 % 32;
}

UNITTEST(algotest) {
	std::vector<int> a;
	for(int i = 0; i < 128*128; ++i) {
		a.push_back(i);
	}

	auto i = sweet::find(a.begin(), a.end(), 1337, 1);
	if(*i != 1337) {
		std::cout<<"Found something wrong "<<*i<<std::endl;
	}
}
