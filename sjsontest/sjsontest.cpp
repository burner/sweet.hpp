#include <iostream>
#include <sjson.hpp>
//#include <unit.hpp>

int main() {
	//if(!Unit::runTests()) {
	//	std::cout<<"found error exiting program"<<std::endl;
	//	return 1;
	//}
	sjson::jsonparser t("hull.js");
	auto r = t.getRoot();
	//std::cout<<t;

	sjson::jsonparser t2("hull2.js");
	r = t2.getRoot();
	std::cout<<t2;

	return 0;
}
