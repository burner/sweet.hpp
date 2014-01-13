#include <iostream>

#include <stringfunctions.hpp>
#include <unit.hpp>

UNITTEST(splittest1) {
	std::string s {"0,1,2,3,4,5,6,7"};

	std::vector<short> v = sweet::split<std::vector<short>>(s, ',');
	for(short i = 0; i < 8; ++i) {
		AS_EQ(i, v[i]);
	}
}

int main() {
	unsigned rslt{sweet::Unit::runTests()};
	if(rslt) {
		std::cout<<rslt<<std::endl;
	}

	std::cout<<"Number of Asserts "<<sweet::Unit::getNumOfAsserts()<<std::endl;

	return 0;
}
