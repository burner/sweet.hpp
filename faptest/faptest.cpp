#include <vector>
#include <random>
#include <bitset>
#include <map>

#include <fap.hpp>
#include <unit.hpp>
#include <logger.hpp>
#include <benchmark.hpp>

UNITTEST(insert) {
	BENCH(insertBen);
	enum {NumElem = 128};

	std::random_device rd;
	//std::mt19937 gen(rd());
	std::mt19937 gen(NumElem);
	std::uniform_int_distribution<> dis(0, NumElem);

	for(size_t j = 1; j < NumElem; ++j) {
		std::bitset<200> t;
		sweet::Fap<int,size_t,200> store;
		for(size_t i = 0; i < j; ++i) {
			int v = dis(gen);
			auto it(store.insert(std::make_pair(v,i)));

			if(t[v]) {
				AS_F(it.second);
			} else {
				AS_T(it.second);
				AS_EQ(it.first->first, v);
				AS_EQ(it.first->second, i);
				t.set(v);
			}
		}
		AS_T(store.size() > 0);

		for(typename sweet::Fap<int,size_t,200>::const_iterator it = store.begin(); it != store.end();) {
			it = store.erase(it);
		}

		AS_EQ(store.size(), 0u);
	}
}

sweet::Fap<int,size_t,1024> getFap(int seed, sweet::Fap<int,size_t,1024>& store) {
	BENCH(benchFap);
	enum {NumElem = 1023};
    std::mt19937 rgen(seed);
	std::mt19937 gen(NumElem);
	std::uniform_int_distribution<> dis(0, NumElem);

	for(size_t i = 0; i < NumElem; ++i) {
		int v = dis(gen);
		auto it(store.insert(std::make_pair(v,i)));
	}

	return store;
}

std::map<int,size_t> getMap(int seed, std::map<int,size_t>& store) {
	BENCH(benchMap);
	enum {NumElem = 1023};
    std::mt19937 rgen(seed);
	std::mt19937 gen(NumElem);
	std::uniform_int_distribution<> dis(0, NumElem);

	for(size_t i = 0; i < NumElem; ++i) {
		int v = dis(gen);
		auto it(store.insert(std::make_pair(v,i)));
	}

	return store;
}

UNITTEST(speed) {
	for(int i = 0; i < 1024*1; ++i) {
		//AS_EQ(getFap(i), getMap(i));
		std::map<int,size_t> map;
		sweet::Fap<int,size_t,1024> fap;
		getFap(i,fap);
		getMap(i,map);

		//AS_T(std::equal(fap.begin(), fap.end(), map.begin(), map.end(), 
		AS_T(std::equal(fap.begin(), fap.end(), map.begin(),
			[](const std::pair<const int,size_t>& a, 
				const std::pair<const int,size_t>& b) 
			{
				return a.first == b.first;
			}
		));
	}
}

UNITTEST(simple) {
	sweet::Fap<int,int,200> store;
	auto it(store.insert(std::make_pair(12, 2)));
	AS_T(it.second);
	AS_EQ(it.first->first, 12);
	AS_EQ(it.first->second, 2);

	auto jt = store.find(12);
	AS_T(jt != store.end());

	const auto store2 = store;
	auto jt2 = store2.find(12);
	AS_T(jt2 != store2.end());
}
