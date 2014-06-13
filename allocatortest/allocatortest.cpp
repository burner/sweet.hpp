#include <vector>
#include <allocator.hpp>
#include <unit.hpp>
#include <logger.hpp>
#include <benchmark.hpp>

// Mallocator
UNITTEST(test1) {
	std::vector<int, sweet::Mallocator<int>> v1;
	v1.push_back(1);
	AS_T(v1[0] == 1);
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
}

// FallbackAllocator
UNITTEST(test4) {
	std::vector<int, 
		sweet::FallbackAllocator<int, 
			sweet::FailAllocator<int>,
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

// BumpAllocator
UNITTEST(test5) {
	sweet::BumpAllocator<int,128> a;

	auto aPtr = a.allocate(4);
	a.deallocate(aPtr,4);
}

UNITTEST(test6) {
	std::vector<int, sweet::BumpAllocator<int> > v1;
	for(int i = 0; i < 1000; ++i) {
		v1.push_back(i);
		for(int j = 0; j < i; ++j) {
			AS_EQ(v1[j], j);
		}
	}
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

int main() {
	unsigned worked = sweet::Unit::runTests();
	std::cout<<worked <<" Number of Asserts "<<
		sweet::Unit::getNumOfAsserts()<<std::endl;

	std::vector<size_t> values {10,24,32,64,128,1024};
	for(size_t i : values) {
		size_t cnt = 5500;
		size_t a = BENCHMARK_CNT(cnt, (fillVectorDeque<std::vector<size_t>>(i)));
		size_t b = BENCHMARK_CNT(cnt, (fillVectorDeque<std::vector<size_t,sweet::Mallocator<size_t>>>(i)));

		typedef std::vector<size_t, sweet::FallbackAllocator<size_t,
				sweet::BumpAllocator<size_t, 2048>,
				sweet::Mallocator<size_t>>
			> FallVector;
		size_t c = BENCHMARK_CNT(cnt, fillVectorDeque<FallVector>(i));

		typedef std::vector<size_t, sweet::Mallocator<size_t>> VectorMalloc;
		size_t d = BENCHMARK_CNT(cnt, fillVectorDeque<VectorMalloc>(i));

		typedef std::vector<size_t, sweet::FallbackAllocator<size_t,
				sweet::FreeVectorAllocator<size_t,
					sweet::SingleBlockAllocator<size_t, sweet::Mallocator<size_t>, 2048>
				>,
				sweet::Mallocator<size_t>>
			> FallMallocVector;
		size_t e = BENCHMARK_CNT(cnt, fillVectorDeque<FallMallocVector>(i));

		format(std::cout, "%10u: %5u %5u %5u %5u %5u\n", i, a, b, c, d, e);
	}

	return 0;
}
