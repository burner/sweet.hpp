#include <cstddef>
#include <iostream>
#include <stdexcept> // optional
#include <unit.hpp>
#include <logger.hpp>
#include <getopts.hpp>

UNITTEST(getopt_test1) {
	const char* argv[3] = {"getoptstext", "--hello", "1337"};
	int hello;
	sweet::getopts("some test", 2, const_cast<char**>(argv), "--hello", &hello);
}
