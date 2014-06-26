#include <vector>
#include <map>
#include <bllocator.hpp>
#include <unit.hpp>
#include <logger.hpp>
#include <benchmark.hpp>

// Mallocator
UNITTEST(test1) {
	sweet::Mallocator m;
	auto ptr = m.allocate(10);
	AS_T(ptr != nullptr);
	m.deallocate(ptr);
}

UNITTEST(test2) {
	typedef sweet::STLAllo<int,sweet::Mallocator> IntAllo;

	std::vector<int, IntAllo> v1;
	for(int i = 0; i < 100; ++i) {
		v1.push_back(i);
		for(int j = 0; j < i; ++j) {
			AS_EQ(v1[j], j);
		}
	}

	std::map<int,int,std::less<int>,IntAllo> m1;
	const int cnt = 100;
	const int step = 2;
	for(int j = 0; j < cnt; j += step) {
		for(int i = 0; i < j; ++i) {
			m1.insert(std::make_pair(i,i));
			auto it = m1.find(i);
			AS_T(it != m1.end());
		}

		for(int i = 0; i < j/2u; ++i) {
			auto it = m1.find(i);
			AS_T(it != m1.end());
			m1.erase(it);
		}
	}
}

UNITTEST(test3) {
	typedef sweet::STLAllo<int, 
		sweet::FallbackAllocator<
			sweet::FailAllocator,
			sweet::Mallocator
		>
	> IntAllo;

	std::vector<int, IntAllo> v1;
	for(int i = 0; i < 100; ++i) {
		v1.push_back(i);
		for(int j = 0; j < i; ++j) {
			AS_EQ(v1[j], j);
		}
	}

	std::map<int,int,std::less<int>,IntAllo> m1;
	const int cnt = 100;
	const int step = 2;
	for(int j = 0; j < cnt; j += step) {
		for(int i = 0; i < j; ++i) {
			m1.insert(std::make_pair(i,i));
			auto it = m1.find(i);
			AS_T(it != m1.end());
		}

		for(int i = 0; i < j/2u; ++i) {
			auto it = m1.find(i);
			AS_T(it != m1.end());
			m1.erase(it);
		}
	}
}


UNITTEST(test4) {
	typedef sweet::STLAllo<int, 
		sweet::FallbackAllocator<
			sweet::StackAllocator<4096>,
			sweet::Mallocator
		>
	> IntAllo;

	std::vector<int, IntAllo> v1;
	for(int i = 0; i < 100; ++i) {
		v1.push_back(i);
		for(int j = 0; j < i; ++j) {
			AS_EQ(v1[j], j);
		}
	}

	std::map<int,int,std::less<int>,IntAllo> m1;
	const int cnt = 100;
	const int step = 2;
	for(int j = 0; j < cnt; j += step) {
		for(int i = 0; i < j; ++i) {
			m1.insert(std::make_pair(i,i));
			auto it = m1.find(i);
			AS_T(it != m1.end());
		}

		for(int i = 0; i < j/2u; ++i) {
			auto it = m1.find(i);
			AS_T(it != m1.end());
			m1.erase(it);
		}
	}
}

UNITTEST(test4_5) {
	sweet::StackAllocator<256> s;
	for(int i = 0; i < 280; ++i) {
		void* a = s.allocate(1);
		if(i < 256) {
			AS_T(a != nullptr);
			AS_T_C(s.allocated(a), [&]() {
				LOG("%d", i);
			});
		} else {
			AS_T(a == nullptr);
			AS_F(s.allocated(a));
		}
	}
}

UNITTEST(test4_10) {
	sweet::PoolAllocator<256> s;
	for(int i = 0; i < 280; ++i) {
		void* a = s.allocate(1);
		if(i < 256) {
			AS_T(a != nullptr);
			AS_T_C(s.allocated(a), [&]() {
				LOG("%d", i);
			});
		} else {
			AS_T(a == nullptr);
			AS_F(s.allocated(a));
		}
	}
}

UNITTEST(test5) {
	typedef sweet::STLAllo<int, 
		sweet::FallbackAllocator<
			sweet::FreeDequeAllocator<sweet::PoolAllocator<4096>>,
			sweet::Mallocator
		>
	> IntAllo;

	std::vector<int, IntAllo> v1;
	for(int i = 0; i < 100; ++i) {
		v1.push_back(i);
		for(int j = 0; j < i; ++j) {
			AS_EQ(v1[j], j);
		}
	}

	std::map<int,int,std::less<int>,IntAllo> m1;
	const int cnt = 100;
	const int step = 2;
	for(int j = 0; j < cnt; j += step) {
		for(int i = 0; i < j; ++i) {
			m1.insert(std::make_pair(i,i));
			auto it = m1.find(i);
			AS_T(it != m1.end());
		}

		for(int i = 0; i < j/2u; ++i) {
			auto it = m1.find(i);
			AS_T(it != m1.end());
			m1.erase(it);
		}
	}
}

template<typename T>
void fillVectorDeque(size_t cnt) {
	//format(std::cout, "%u\n", cnt);
	T v1;
	size_t step = cnt / 10u;
	for(size_t j = step; j < cnt; j += step) {
		for(size_t i = 0; i < j; ++i) {
			v1.push_back(i);
			if(v1.back() != i) {
				format(std::cout, "%u != %u", v1.back(), i);
				throw std::exception();
			}
		}

		for(size_t i = 0; i < j/2u; ++i) {
			v1.pop_back();
		}
	}
}

template<typename T>
void fillMap(size_t cnt) {
	//format(std::cout, "%u\n", cnt);
	T v1;
	size_t step = cnt / 10u;
	for(size_t j = step; j < cnt; j += step) {
		for(size_t i = 0; i < j; ++i) {
			v1.insert(std::make_pair(i,i));
		}

		for(size_t i = 0; i < j/2u; ++i) {
			auto it = v1.find(i);
			v1.erase(it);
		}
	}
}

int main() {
	unsigned worked = sweet::Unit::runTests();
	std::cout<<worked <<" Number of Asserts "<<
		sweet::Unit::getNumOfAsserts()<<std::endl;

	LOG("vector");
	//std::vector<size_t> values {10,24,32,64,128,256,512};
	std::vector<size_t> values {10,24,32,64};
	size_t cnt = 5500;
	for(size_t i : values) {
		double a = BENCHMARK_CNT(cnt, (fillVectorDeque<std::vector<size_t>>(i)));
		double b = BENCHMARK_CNT(cnt, (fillVectorDeque<std::vector<size_t,sweet::STLAllo<size_t,sweet::Mallocator>>>(i)));

		typedef std::vector<size_t, 
			sweet::STLAllo<
				size_t,
				sweet::FallbackAllocator<
					sweet::StackAllocator<4096>,
					sweet::Mallocator
				>
			>
		> StackVector;
		
		double c = BENCHMARK_CNT(cnt, fillVectorDeque<StackVector>(i));

		typedef std::vector<size_t, 
			sweet::STLAllo<
				size_t,
				sweet::FallbackAllocator<
					sweet::PoolAllocator<4096>,
					sweet::Mallocator
				>
			>
		> PoolVector;
		
		double d = BENCHMARK_CNT(cnt, fillVectorDeque<PoolVector>(i));

		format(std::cout, "%10u: %9.4f %9.4f %9.4f %9.4f\n", i, a, b, c, d);
	}

	LOG("\nmap");
	for(size_t i : values) {
		double a = BENCHMARK_CNT(cnt, (fillMap<std::map<size_t,size_t>>(i)));
		double b = BENCHMARK_CNT(cnt, (fillMap<std::map<size_t,size_t, std::less<size_t>,
			sweet::STLAllo<std::pair<size_t,size_t>,sweet::Mallocator>>>(i)));

		typedef std::map<size_t,size_t,std::less<size_t>,
			sweet::STLAllo<
				std::pair<size_t,size_t>,
				sweet::FallbackAllocator<
					sweet::StackAllocator<4096>,
					sweet::Mallocator
				>
			>
		> StackMap;
		
		double c = BENCHMARK_CNT(cnt, fillMap<StackMap>(i));

		typedef std::map<size_t,size_t,std::less<size_t>,
			sweet::STLAllo<
				std::pair<size_t,size_t>,
				sweet::FallbackAllocator<
					sweet::PoolAllocator<4096>,
					sweet::Mallocator
				>
			>
		> PoolMap;
		
		double d = BENCHMARK_CNT(cnt, fillMap<PoolMap>(i));

		format(std::cout, "%10u: %9.4f %9.4f %9.4f %9.4f\n", i, a, b, c, d);
	}

	return 0;
}
