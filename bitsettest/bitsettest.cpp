#include <vector>

#include <bitset.hpp>
#include <unit.hpp>
#include <benchmark.hpp>
#include <limits>
#include <bitset>

UNITTEST(simpletest) {
	for(uint32_t i = 0; i < std::numeric_limits<uint32_t>::max()/2; i+= 13357) {
		sweet::Bitset<uint32_t> b(i);
		std::bitset<32> b2(i);

		for(size_t j = 0; j < 32; ++j) {
			AS_EQ(b[j], b2[j]);
		}
	}
}

int main() {
	auto t(sweet::Unit::runTests());
	sweet::Benchmark::printResults();
	return t;
}
