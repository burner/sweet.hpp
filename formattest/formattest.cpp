#include <iostream>
#include <format.hpp>

class Foo {
};

int main() {
	format(std::cout, "<<%s<<\n", "Foo");
	Foo* f;
	format(std::cout, "%p\n", f);
	return 0;
}
