#include "conv.hpp"
#include "unit.hpp"

UNITTEST(toString) {
	AS_EQ("1337", to(1337));
	AS_EQ("1337.000000", to(1337.0));
	AS_EQ(1337, to<int>("1337"));
	AS_EQ(1337.000000, to<double>("1337.0"));
}

int main() {
	return !Unit::runTests();
}
