#include <string>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include "../benchmark.hpp"
#include "../format.hpp"
#include "../unit.hpp"

char random_char() {
	return ( rand() % ( 26 ) ) + 97;
}

std::string randomString() {
	std::string str;
	std::generate_n( std::back_inserter( str ), 10, random_char);
	return str;
}

UNITTEST(setVsUnordered) {
	size_t cnt = 1;
	const size_t rounds = 1000;
	for(size_t j = 1; j < rounds; j+=10) {
		cnt+=j;
		srand(cnt);
		sweet::Bench b;
		std::set<std::string> s;
		for(size_t i = 0; i < cnt; ++i) {
			s.insert(randomString());
		}
		b.stop();

		srand(cnt);
		sweet::Bench b2;
		std::unordered_set<std::string> u;
		for(size_t i = 0; i < cnt; ++i) {
			u.insert(randomString());
		}
		b2.stop();
		format(std::cout, "%8u %4u %4u\n", cnt, b.milli(), b2.milli());
	}
}
