#include <vector>

#include <logger.hpp>
#include <unit.hpp>
#include <benchmark.hpp>
#include <bitset.hpp>

UNITTEST(size) {
	AS_EQ(sizeof(sweet::Bitset<uint8_t>), 1u);
	AS_EQ(sizeof(sweet::Bitset<uint16_t>), 2u);
	AS_EQ(sizeof(sweet::Bitset<uint32_t>), 4u);
	AS_EQ(sizeof(sweet::Bitset<uint64_t>), 8u);

	sweet::Bitset<uint8_t> eight;
	AS_EQ(eight.size(), 8u);

	sweet::Bitset<uint16_t> sixteen;
	AS_EQ(sixteen.size(), 16u);

	sweet::Bitset<uint32_t> thirtytwo;
	AS_EQ(thirtytwo.size(), 32u);

	sweet::Bitset<uint64_t> fourtysix;
	AS_EQ(fourtysix.size(), 64u);
}

size_t countBits(const uint64_t n) {
	int cnt = 0;
	for(uint64_t j = 0; j < 64u; ++j) {
		if(n & 1ul << j) {
			++cnt;
		}
	}	
	return cnt;
}

UNITTEST(access8) {
	std::vector<sweet::Bitset<uint8_t>> b8;
	for(uint16_t i = 0; i <= std::numeric_limits<uint8_t>::max(); ++i) {
		b8.push_back(sweet::Bitset<uint8_t>(i));
		AS_EQ_C(b8.back().count(), countBits(i), [&]() {
			LOG("i %s, b8 %s, countBits %s", static_cast<uint32_t>(i), b8.back().count(), countBits(i));
		});
	}

	SECTION("const") {
		for(const auto& it : b8) {
			for(size_t i = 0; i < 8u; ++i) {
				AS_EQ(it[i], it.test(i));
			}
		}
	}

	SECTION("non-const") {
		for(auto& it : b8) {
			for(size_t i = 0; i < 8u; ++i) {
				typename sweet::Bitset<uint8_t>::Reference iit = it[i];
				AS_EQ(iit, it.test(i));
				iit = iit = true;
				AS_EQ(iit, it.test(i));
				AS_EQ(iit, true);
				iit = iit = false;
				AS_EQ(iit, it.test(i));
				AS_EQ(iit, false);
			}
		}
	}

	SECTION("flip") {
		for(auto& it : b8) {
			for(size_t i = 0; i < 8u; ++i) {
				bool old = it[i];
				it.flip(i);
				AS_NEQ(old, it[i]);
			}
		}
	}

	SECTION("flip_not_ant_friends") {
		for(auto& it : b8) {
			auto nit = ~it;

			AS_EQ(it | nit, sweet::Bitset<uint8_t>(std::numeric_limits<uint8_t>::max()));
			AS_EQ(it ^ nit, sweet::Bitset<uint8_t>(std::numeric_limits<uint8_t>::max()));
			AS_EQ(it ^ nit ^ sweet::Bitset<uint8_t>(std::numeric_limits<uint8_t>::max()), 
				sweet::Bitset<uint8_t>(0));
			AS_EQ(it & nit, sweet::Bitset<uint8_t>(0));
		}
	}
}

UNITTEST(access32) {
	std::vector<sweet::Bitset<uint32_t>> b32;
	for(uint64_t i = 0; i <= std::numeric_limits<uint32_t>::max()/2; i += 133734373) {
		b32.push_back(sweet::Bitset<uint32_t>(i));
		AS_EQ_C(b32.back().count(), countBits(i), [&]() {
			LOG("i %s, b32 %s, countBits %s", static_cast<uint32_t>(i), b32.back().count(), countBits(i));
		});
	}

	SECTION("const") {
		for(const auto& it : b32) {
			for(size_t i = 0; i < 32u; ++i) {
				AS_EQ(it[i], it.test(i));
			}
		}
	}

	SECTION("non-const") {
		for(auto& it : b32) {
			for(size_t i = 0; i < 32u; ++i) {
				typename sweet::Bitset<uint32_t>::Reference iit = it[i];
				AS_EQ(iit, it.test(i));
				iit = iit = true;
				AS_EQ(iit, it.test(i));
				AS_EQ(iit, true);
				iit = iit = false;
				AS_EQ(iit, it.test(i));
				AS_EQ(iit, false);
			}
		}
	}

	SECTION("flip") {
		for(auto& it : b32) {
			for(size_t i = 0; i < 32u; ++i) {
				bool old = it[i];
				it.flip(i);
				AS_NEQ(old, it[i]);
			}
		}
	}

	SECTION("flip_not_ant_friends") {
		for(auto& it : b32) {
			auto nit = ~it;

			AS_EQ(it | nit, sweet::Bitset<uint32_t>(std::numeric_limits<uint32_t>::max()));
			AS_EQ(it ^ nit, sweet::Bitset<uint32_t>(std::numeric_limits<uint32_t>::max()));
			AS_EQ(it ^ nit ^ sweet::Bitset<uint32_t>(std::numeric_limits<uint32_t>::max()), 
				sweet::Bitset<uint32_t>(0));
			AS_EQ(it & nit, sweet::Bitset<uint32_t>(0));
		}
	}
}


int main() {
	auto t(sweet::Unit::runTests());
	if(t) {
		WARN("some tests failed");
	} else {
		LOG("check %s asserts", sweet::Unit::getNumOfAsserts());
	}

	/*for(int i = 0; i < 256; ++i) {
	}
	std::cout<<std::endl;
	*/
	return t;
}
