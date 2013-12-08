#include <decimal.hpp>
#include <unit.hpp>

UNITTEST(decimal1) {
	Decimal d;

	auto d1(d + 1);
	auto d2(d + 1.0);

	d1 += 2;
	std::cout<<d<<std::endl;
	std::cout<<d1<<std::endl;
	std::cout<<d2<<std::endl;
	Decimal d3(0.1);
	std::cout<<d3<<std::endl;

	d1 = d3;
	std::cout<<d1<<std::endl;
}

int main() {
	Unit::runTests();
	return 0;
}
