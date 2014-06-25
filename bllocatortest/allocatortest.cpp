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

int main() {
	unsigned worked = sweet::Unit::runTests();
	std::cout<<worked <<" Number of Asserts "<<
		sweet::Unit::getNumOfAsserts()<<std::endl;
	return 0;
}
