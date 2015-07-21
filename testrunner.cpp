#include "unit.hpp"
#include "format.hpp"

int main() {
	unsigned c = 0;
	if((c = sweet::Unit::runTests("TheNameOfTheFileWithTheBenchmarkResults"))) {
		format(std::cout, "%u errors occured", c);
	}

	return 0;
}
