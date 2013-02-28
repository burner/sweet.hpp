//#include "format.hpp"
//#include "logger.hpp"
#include <iostream>
#include <iostream> // optional
#include <stdexcept> // optional
#include "unit.hpp"
	
UNITTEST(fancyname) {
	//std::ofstream unittestlog("testlog.tst");	// optional
	// this->setOutputStream(&unittestlog); 	// optional
	AS_T(true);
	AS_F(4!=4);
	AS_EQ(42, 42);
	AS_NEQ(42, 43);

	if(AS_T(true)) {
		// do a test that is only possible if the previous test passed
	}
}

UNITTEST(foo, 66) {
	AS_T(4==4);
}

UNITTEST(bar) {
	//throw std::logic_error("something went wrong");
}

UNITTEST(bar2) {
	//throw std::string("something went wrong");
}

int main() {
	if(!Unit::runTests()) {
		std::cerr<<"Some errors occoured"<<std::endl;
	}
	//formatToStream(std::cout, "hello print world %10d\n", 10);
	//format(std::cout, "%18.2E %s\n", 10.123, "hello world");
	//format(std::cout, "Helo\n");
	//std::cout<<format("hello wo")<<std::endl;
	//std::cout<<format("hello wo %d", 1337)<<std::endl;
	//std::cout<<format("hello wo %d %.5E", 1337, 1.234345346)<<std::endl;
	////std::cout<<"hello print world "<<10<<std::endl;
	//LOG();
	//WARN("Hello warnign");
	//LOG("Hello warnign %s", std::string("foobar"));
	//LOG("Hello warnign %p", std::string("foobar"));
	//LOG("Hello warnign %p", "foobar");
	return 0;
}
