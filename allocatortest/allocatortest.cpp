#include <vector>
#include <allocator.hpp>
#include <unit.hpp>
#include <logger.hpp>

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
		AS_EQ(v1.back(), i);
	}
}


int main() {
	unsigned worked = sweet::Unit::runTests();
	std::cout<<worked <<" Number of Asserts "<<
		sweet::Unit::getNumOfAsserts()<<std::endl;
	return 0;
}
