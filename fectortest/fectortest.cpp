#include <fector.hpp>
#include <unit.hpp>

UNITTEST(inistan) {
	sweet::Fector<int,100> f;
}

int main() {
	auto t(sweet::Unit::runTests());
	return t;
}
