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
	AS_EQ(f.size(), 1u);

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

	auto rit(f.rbegin());
	AS_EQ(*rit, 4);
	--rit;
	AS_EQ(*rit, 3);
	--rit;
	AS_EQ(*rit, 1);
	AS_T(rit == f.rend());

	auto it(f.begin());
	AS_EQ(*it, 1)
	++it;
	AS_EQ(*it, 3);
	it = f.insert(it, 2);
	AS_EQ(*it, 2);
	AS_EQ(*f.begin(), 1);
	++it;
	AS_EQ(*it, 3);
	AS_T(f.size() == 4);

	for(int i = 1; i <= 4; ++i) {
		AS_EQ(f[i-1], i);
	}
}

UNITTEST(insertT2) {
	sweet::Fector<short,20> f;
	f.insert(f.begin(), 1);
	AS_EQ(f[0], 1);
	AS_EQ(*f.begin(), 1);

	auto it = std::lower_bound(f.begin(), f.end(), 0);
	f.insert(it, 0);
	AS_EQ(f[0], 0);
	AS_EQ(*f.begin(), 0);
	AS_EQ(f[1], 1);
	AS_EQ(*(f.begin()+1), 1);
}

UNITTEST(insertT3) {
	sweet::Fector<short,20> f;
	f.insert(f.end(), 1);
	AS_EQ(f[0], 1);
	AS_EQ(*f.begin(), 1);

	auto it = std::lower_bound(f.begin(), f.end(), 0);
	f.insert(it, 0);
	AS_EQ(f[0], 0);
	AS_EQ(*f.begin(), 0);
	AS_EQ(f[1], 1);
	AS_EQ(*(f.end()-1), 1);
}

UNITTEST(eraseT1) {
	sweet::Fector<int,32> f;
	for(int i = 0; i < 32; ++i) f.push_back(i);

	f.erase(f.begin());
	AS_EQ(f.size(), 31u);
	for(int i = 0; i < 31; ++i) {
		AS_EQ(f[i], i+1);
	}
	f.erase(f.end());
	AS_EQ(f.size(), 30u);
	for(int i = 0; i < 30; ++i) {
		AS_EQ(f[i], i+1);
	}

	f.erase(f.begin()+3);
	AS_EQ(f.size(), 29u);
	int v = 1;
	for(int i = 0; i < 29; ++i) {
		if(i != 3) {
			AS_EQ(f[i], v);
			++v;
		} else {
			v+=2;
		}
	}
}

sweet::Fector<int,32> getFector(int i) {
	sweet::Fector<int,32> ret;
	for(int j = 0; j < i; ++j) {
		ret.push_back(j);
	}
	return ret;
}

UNITTEST(eraseT2) {
	BENCH(eraseT2);
	for(int i = 0; i < 32; ++i) {
		for(int j = 0; j < i; ++j) {
			auto fec = getFector(i);
			auto oldSize = fec.size();
			fec.erase(fec.begin()+j);
			AS_EQ(oldSize, fec.size()+1);

			int idx(0);
			for(int k = 0; k < i-1;) {
				if(idx == j) {
					++k;
				} else {
					AS_EQ(fec[idx], k);
					++k;
					++idx;
				}
			}
		}
	}
}

UNITTEST(ctor) {
	size_t cnt{8};
	sweet::Fector<int,24> f(cnt, 1337);
	AS_EQ(f.size(), cnt);
	for(size_t i = 0; i < cnt; ++i) {
		AS_EQ(f[i], 1337);
	}

	f.resize(14, 1337);
	AS_EQ(f.size(), 14u);
	for(size_t i = 0; i < 14; ++i) {
		AS_EQ(f[i], 1337);
	}

	f.resize(4, 1337);
	AS_EQ(f.size(), 4u);
	for(size_t i = 0; i < 4; ++i) {
		AS_EQ(f[i], 1337);
	}
}

UNITTEST(push_back_test1) {
	sweet::Fector<int,4> f;
	f.push_back(0);
	f.push_back(1);
	f.push_back(2);
	f.push_back(3);

	bool t = false;
	try {
		f.push_back(4);
	} catch(std::out_of_range& e) {
		t = true;
	}
	AS_T(t);
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

	int sum2{0};
	for(int i : store) {
		sum2 += i;
	}

	ASSERT_EQ(sum, sum2);

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

/*
UNITTEST(speed) {
	for(int i = 0; i < 128; ++i) {
		AS_EQ(sumVector(), sumFector());
	}
}
*/

struct At {
	int a;
	double b;
	inline At() {}
	inline At(int a, double b) : a(a), b(b) {}
};

UNITTEST(emplace) {
	sweet::Fector<At,200> store;
	store.emplace(1, 1.1);
	AS_EQ(store[0].a, 1);
	AS_EQ(store[0].b, 1.1);
}

int main() {
	auto t(sweet::Unit::runTests());
	sweet::Benchmark::printResults();
	return t;
}
