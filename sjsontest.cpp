#include <iostream>
#include "sjson.hpp"
#include "unit.hpp"

int main() {
	if(!Unit::runTests()) {
		std::cout<<"found error exiting program"<<std::endl;
		return 1;
	}
	jsonparser t("hull.js");
	auto r = t.getRoot();

	return 0;
}
