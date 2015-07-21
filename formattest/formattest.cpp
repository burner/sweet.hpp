#include <iostream>
#include <format.hpp>
#include <unit.hpp>

class Foo {
};

UNITTEST(formattest) {
	format(std::cout, "<<%s<<\n", "Foo");
	Foo* f;
	format(std::cout, "%p\n", f);
}
