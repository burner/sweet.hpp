#include <vector>

#include <fector.hpp>
#include <unit.hpp>
#include <benchmark.hpp>

UNITTEST(vect) {
	std::vector<int> f;
	f.emplace_back(2);
}

UNITTEST(inistan) {
	sweet::Fector<int,100> f;
	AS_T(f.empty());
	f.push_back(1);
	AS_F(f.empty());
	AS_EQ(f.size(), 1);

	AS_EQ(f.front(), f.back());

	for(auto it : f) {
		AS_EQ(it, 1);		
	}

	const auto f2 = f;
	auto it = f2.begin();
	AS_EQ(*it, 1);		
}

UNITTEST(insertT) {
	sweet::Fector<int,4> f;
	f.push_back(1);
	AS_T(f.size() == 1);
	f.push_back(3);
	AS_T(f.size() == 2);
	f.push_back(4);
	AS_T(f.size() == 3);

	auto it(f.begin());
	++it;
	AS_EQ(*it, 3);
	it = f.insert(it, 2);
	AS_EQ(*it, 2);
	++it;
	AS_EQ(*it, 3);
	AS_T(f.size() == 4);
}

UNITTEST(ctor) {
	size_t cnt{8};
	sweet::Fector<int,24> f(cnt, 1337);
	AS_EQ(f.size(), cnt);
	for(size_t i = 0; i < cnt; ++i) {
		AS_EQ(f[i], 1337);
	}

	f.resize(14, 1337);
	AS_EQ(f.size(), 14);
	for(size_t i = 0; i < 14; ++i) {
		AS_EQ(f[i], 1337);
	}

	f.resize(4, 1337);
	AS_EQ(f.size(), 4);
	for(size_t i = 0; i < 4; ++i) {
		AS_EQ(f[i], 1337);
	}
}


int z[128] = {102, 72, 44, 32, 120, 96, 103, 73, 126, 14, 116, 6, 53, 25, 36, 105, 
	78, 90, 27, 101, 46, 77, 86, 1, 122, 75, 109, 20, 59, 31, 52, 23, 9, 16, 
	74, 34, 81, 0, 13, 41, 17, 124, 121, 71, 108, 87, 93, 70, 67, 118, 83, 80, 
	15, 123, 94, 85, 60, 18, 50, 115, 63, 35, 82, 28, 21, 57, 68, 88, 54, 24, 
	42, 11, 3, 40, 7, 66, 99, 114, 5, 117, 47, 97, 127, 65, 69, 39, 2, 4, 92, 
	19, 76, 29, 61, 98, 111, 79, 112, 45, 104, 125, 119, 100, 107, 43, 8, 10, 
	95, 56, 51, 58, 64, 38, 48, 30, 37, 55, 89, 12, 91, 22, 62, 110, 33, 84, 
	26, 49, 113, 106};

int sumVector() {
	BENCH(sumVector);
	std::vector<int> store;
	const size_t elem{128};
	for(size_t i = 0; i < elem; ++i) {
		store.push_back(z[i]);
	}

	int sum{0};
	for(size_t i = 0; i < elem; ++i) {
		sum += store[i];
	}

	return sum;
}

int sumFector() {
	BENCH(sumFector);
	sweet::Fector<int,200> store;
	const size_t elem{128};
	for(size_t i = 0; i < elem; ++i) {
		store.push_back(z[i]);
	}

	int sum{0};
	for(size_t i = 0; i < elem; ++i) {
		sum += store[i];
	}

	return sum;
}

UNITTEST(speed) {
	for(int i = 0; i < 128*128; ++i) {
		AS_EQ(sumVector(), sumFector());
	}
}

int main() {
	auto t(sweet::Unit::runTests());
	sweet::Benchmark::printResults();
	return t;
}
