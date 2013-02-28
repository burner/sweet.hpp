#include <vector>
#include <iostream>
#include <chrono>
#include <set>
#include "parallelalgo.hpp"
#include "logger.hpp"
#include "benchmark.hpp"

void fun(int& a) {
	a *= 5;
}

void fun2(int in, std::insert_iterator<std::set<int>>& iter) {
	 *iter++ = (in) * 127 % 32;
}

int fun2_5(int in) {
	return in * 127 % 32;
}

int main() {
	std::vector<int> v(1, 1);

	Bench a;
	css::for_each(v.begin(), v.end(), [](int& a) {
		auto n = std::chrono::system_clock::now().time_since_epoch().count();
		a *= 5 * n;
	});
	a.stop();
	LOG("css::for_each took %u milliseconds", a.milli());

	for(int i = 0; i < 10; ++i) {
		Bench b;
		std::set<int> rslt;
		css::mapReduce<
			std::vector<int>::iterator, 
			std::insert_iterator<std::set<int>>,
			std::set<int>
		>(v.begin(), v.end(), std::inserter(rslt,rslt.begin()), fun2, 2);
		b.stop();

		Bench c;
		std::set<int> rslt2;
		std::transform(v.begin(), v.end(), std::inserter(rslt2,
					rslt2.begin()), fun2_5);
		c.stop();

		LOG("css::map_reduce took %u milliseconds", b.milli());
		LOG("transform took %u milliseconds", c.milli());
	}

	//LOG("v.size(%u) rslt.size(%u)", v.size(), rslt.size());

	return 0;
}
