#include <iostream>
#include <format.hpp>

int main() {
	format(std::cout, "<<%s<<\n", "Foo");
	return 0;
}
