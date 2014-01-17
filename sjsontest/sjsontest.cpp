#include <iostream>
#include <sjson.hpp>
#include <unit.hpp>

UNITTEST(test1) {
	sweet::jsonparser t("hull.js");
	auto r = t.getRoot();
	AS_EQ(r->access("Config.system.numNodes")->getInt(), 32);
}

int main() {
	unsigned c = sweet::Unit::runTests();
	if(c) {
		std::cout<<"found error exiting program "<<c<<std::endl;
		return 1;
	}
	//std::cout<<t;

	sweet::jsonparser t2("hull2.js");
	auto r = t2.getRoot();
	std::cout<<t2;

	return 0;
}
