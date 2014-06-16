#include <vector>
#include <map>
#include <allocator.hpp>
#include <unit.hpp>
#include <logger.hpp>
#include <benchmark.hpp>

// Mallocator
UNITTEST(test1) {
	std::vector<int, sweet::Mallocator<int>> v1;
	v1.push_back(1);
	AS_T(v1[0] == 1);

	typedef typename sweet::Mallocator<int>::rebind<float> FA;
	FA fa;
}

UNITTEST(test2) {
	std::vector<std::string, sweet::Mallocator<int>> v1;
	v1.push_back("hello world");
	v1.push_back("hello robert");
	v1.push_back("world robert");

	AS_T(v1[0] == "hello world");
	AS_T(v1[1] == "hello robert");
	AS_T(v1[2] == "world robert");
}

// FailAllocator
UNITTEST(test3) {
	std::vector<int, sweet::FailAllocator<int>> v1;
	bool failed = false;
	try {
		v1.push_back(1337);
	} catch(...) {
		failed = true;
	}
	AS_T(failed);

	typedef typename sweet::FailAllocator<int>::rebind<float> FA;
	FA fa;
}

// FallbackAllocator
UNITTEST(test4) {
	typedef sweet::FallbackAllocator<int, sweet::FailAllocator, sweet::Mallocator> FA;
	FA fa;
	int* a = fa.allocate(1);
	AS_T(a != nullptr);

	FA::rebind<float>::other faf;
	//FAF faf;
	float* b = faf.allocate(1);
	AS_T(b != nullptr);

	std::vector<int, 
		sweet::FallbackAllocator<int, 
			sweet::FailAllocator,
			sweet::Mallocator
		>
	> v1;
	for(int i = 0; i < 1000; ++i) {
		v1.push_back(i);
		for(int j = 0; j < i; ++j) {
			AS_EQ(v1[j], j);
		}
	}
}
// BumpAllocator
UNITTEST(test5) {
	sweet::BumpAllocator<int,128> a;

	auto aPtr = a.allocate(4);
	a.deallocate(aPtr,4);

	typedef typename sweet::BumpAllocator<int>::rebind<float> FA;
	FA fa;
}

UNITTEST(test6) {
	std::vector<int, sweet::BumpAllocator<int,4096> > v1;
	for(int i = 0; i < 1000; ++i) {
		v1.push_back(i);
		for(int j = 0; j < i; ++j) {
			AS_EQ(v1[j], j);
		}
	}

	sweet::BumpAllocator<int>::DefaultType<float> floatAllo;

}

// FreeStackAllocator
UNITTEST(test7) {
	std::vector<int, 
		sweet::FreeVectorAllocator<int, 
			sweet::Mallocator<int>
		>
	> v1;
	for(int i = 0; i < 1000; ++i) {
		v1.push_back(i);
		for(int j = 0; j < i; ++j) {
			AS_EQ(v1[j], j);
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

	std::vector<size_t> values {10,24,32,64,128,1024};
	for(size_t i : values) {
		size_t cnt = 5500;
		double a = BENCHMARK_CNT(cnt, (fillVectorDeque<std::vector<size_t>>(i)));
		double b = BENCHMARK_CNT(cnt, (fillVectorDeque<std::vector<size_t,sweet::Mallocator<size_t>>>(i)));

		typedef std::vector<size_t, sweet::FallbackAllocator<size_t,
				sweet::BumpAllocator<size_t,4096>,
				sweet::Mallocator> 
		> FallVector;
		/*
		double c = BENCHMARK_CNT(cnt, fillVectorDeque<FallVector>(i));

		typedef std::vector<size_t, sweet::Mallocator<size_t>> VectorMalloc;
		double d = BENCHMARK_CNT(cnt, fillVectorDeque<VectorMalloc>(i));

		typedef std::vector<size_t, sweet::FallbackAllocator<size_t,
				sweet::FreeVectorAllocator<size_t,
					sweet::SingleBlockAllocator<size_t, sweet::Mallocator<size_t>, 2048>
				>,
				sweet::Mallocator<size_t>>
			> FallMallocVector;
		double e = BENCHMARK_CNT(cnt, fillVectorDeque<FallMallocVector>(i));

		format(std::cout, "%10u: %7.4f %7.4f %7.4f %7.4f %7.4f\n", i, a, b, c, d, e);
		*/
	}

	for(size_t i : values) {
		/*
		size_t cnt = 5500;
		double a = BENCHMARK_CNT(cnt, (fillMap<std::map<size_t,size_t>>(i)));
		double b = BENCHMARK_CNT(cnt, (fillMap<std::map<size_t,size_t,
			std::less<size_t>, sweet::Mallocator<std::pair<const size_t,size_t>>>>(i)));

		typedef std::map<size_t,size_t, std::less<size_t>,
			sweet::FallbackAllocator<std::pair<const size_t,size_t>,
				sweet::BumpAllocator<std::pair<const size_t,size_t>, 2048>,
				sweet::Mallocator<std::pair<const size_t,size_t>>
			>
		> FallVector;
		double c = BENCHMARK_CNT(cnt, fillMap<FallVector>(i));

		typedef std::map<size_t,size_t, sweet::Mallocator<std::pair<const size_t,size_t>> VectorMalloc;
		double d = BENCHMARK_CNT(cnt, fillMap<VectorMalloc>(i));

		typedef std::map<size_t,size_t, sweet::FallbackAllocator<std::pair<const size_t,size_t>,
				sweet::FreeVectorAllocator<std::pair<const size_t,size_t>,
					sweet::SingleBlockAllocator<std::pair<const size_t,size_t>, 
						sweet::Mallocator<std::pair<const size_t,size_t>>, 2048>
				>,
				sweet::Mallocator<std::pair<const size_t,size_t>>>
			> FallMallocVector;
		double e = BENCHMARK_CNT(cnt, fillMap<FallMallocVector>(i));

		format(std::cout, "%10u: %5.4f %5.4f %5.4f %5.4f %5.4f\n", i, a, b, c, d, e);
		*/
	}

	return 0;
}
