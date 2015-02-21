#include <string>
#include <sstream>
#include <ostream>
#include <base64.hpp>
#include <unit.hpp>
#include <format.hpp>

UNITTEST(simpleTest) {
	std::string s("HelloWorld");
	std::stringstream ss, ss2, ss3;
	ss<<s;
	encodeBase64(ss, ss2);
	decodeBase64(ss2, ss3);

	std::string s2;
	ss3>>s2;

	AS_EQ(s, s2);
}

int main() {
	unsigned c = 0;
	if((c = sweet::Unit::runTests("TheNameOfTheFileWithTheBenchmarkResults"))) {
		format(std::cout, "%u errors occured", c);
	}

	return 0;
}
