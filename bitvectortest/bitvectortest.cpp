#include "benchmark.hpp"
#include "bitvector.hpp"
#include "unit.hpp"
#include "logger.hpp"
#include "format.hpp"
#include <iostream>
#include <bitset>
#include <vector>

UNITTEST(setTest) {
	BitVector bv;
	size_t upTo = 128;
	for(size_t i = 0; i < upTo; ++i) {
		ASSERT_F_MSG(bv[i], format("%u", i));
	}

	for(size_t i = 0; i < upTo; ++i) {
		bv.set(i);
		for(size_t j = 0; j <= i; ++j) {
			ASSERT_T_MSG(bv[j], format("%u %u", j, bv.threeTwo));
		}

		for(size_t j = i+1; j < upTo; ++j) {
			ASSERT_F_MSG(bv[j], format("i %u + j %u = %u | %u", i, j, i + j, bv.threeTwo));
		}
	}
}

int main() {
	unsigned c = 0;
	if((c = Unit::runTests())) {
		WARN("%u errors occured", c);
	}

	Bench a;
	size_t nTests = 1<<22;
	const size_t mod = 32;
	size_t ac = 0;
	size_t bc = 0;
	size_t cc = 0;
	for(size_t i = 0; i < nTests; ++i) {
		BitVector b;
		for(size_t j = 0; j < mod; ++j) {
			b.set(i * (j+1) % mod);
		}
		for(size_t j = 0; j < mod; ++j) {
			if(b[j]) ++ac;	
		}
	}
	a.stop();
	std::cout<<a.milli()<<" ";

	Bench b;
	for(size_t i = 0; i < nTests; ++i) {
		std::bitset<mod> bs;
		for(size_t j = 0; j < mod; ++j) {
			bs.set(i * (j+1) % mod);
		}
		for(size_t j = 0; j < mod; ++j) {
			if(bs[j]) ++bc;	
		}
	}
	b.stop();
	std::cout<<b.milli()<<" ";

	Bench d;
	for(size_t i = 0; i < nTests; ++i) {
		std::vector<bool> bs(mod, false);
		for(size_t j = 0; j < mod; ++j) {
			bs[i * (j+1) % mod] = true;
		}
		for(size_t j = 0; j < mod; ++j) {
			if(bs[j]) ++cc;	
		}
	}
	d.stop();
	std::cout<<d.milli()<<std::endl;
	ASSERT_EQ(ac, bc);
	ASSERT_EQ(bc, cc);
}
