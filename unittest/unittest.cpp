//#include "format.hpp"
//#include "logger.hpp"
#include <cstddef>
#include <iostream>
//#include <iostream> // optional
#include <stdexcept> // optional
#include <unit.hpp>
#include <logger.hpp>

UNITTEST(fancyname) {
	//std::ofstream unittestlog("testlog.tst");	// optional
	// this->setOutputStream(&unittestlog); 	// optional
	AS_T(true);
	AS_F(4!=4);
	AS_EQ(42, 42);
	AS_NEQ(42, 43);

	int a = 123;

	AS_T(true)
	AS_T_MSG(true, format("foo %d", a))
	LOG("Hello");
	AS_T(false)
	LOG();
}

UNITTEST(magic) {
	//std::ofstream unittestlog("testlog.tst");	// optional
	// this->setOutputStream(&unittestlog); 	// optional
	int a(1337);
	AS_T(true);
	AS_F(4!=4);
	AS_T_C(false, [&](){
		std::cout<<a<<std::endl;
	});
	LOG();
}

UNITTEST(foo, 66) {
	AS_T(4==4);
}

UNITTEST(foon, 66666, "-O3") {
	AS_T(4==4);
}

UNITTEST(bar) {
	//throw std::logic_error("something went wrong");
}

int someAwesomeFunction(int a, int b, int c) {
	return ((a + b) * (a + b)) % c;
}

UNITTEST(someAwesomeFunctionTest) {
	std::stringstream test;
	test<<std::fixed<<std::setprecision(6);
    std::mt19937 rgen(123);
	auto gen = RANDOMIZED_TEST(
		[&](int a, int b, float c) {
			std::string rslt = format("%5d %8.5f %10d", a, c, b);
			test.str("");
			test<<a;
			AS_NEQ_C(rslt.find(test.str()), std::string::npos,
				[&]() { std::cout<<rslt<<" "<<test.str()<<std::endl; }
			);
			test.str("");
			test<<b;
			AS_NEQ_C(rslt.find(test.str()), std::string::npos,
				[&]() { std::cout<<rslt<<" "<<test.str()<<std::endl; }
			);
		}, 
		rgen, sweet::Unit::Gen<int>(-1024,1024), 
		sweet::Unit::Gen<int>(1, 128), sweet::Unit::Gen<float>(-12, 12)
	);
	gen(1024*1024);
}

int main() {
	unsigned c = 0;
	if((c = sweet::Unit::runTests("TheNameOfTheFileWithTheBenchmarkResults"))) {
		WARN("%u errors occured", c);
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
	//ASSERT_T_C(false, [&]() {
		//std::cout<<"I should die here "<<c<<std::endl;
	//});
	LOG("Hello warning %p", std::string("foobar"));
	//LOG("Hello warnign %p", "barfoo");
	return 0;
}
