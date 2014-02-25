#include <fector.hpp>
#include <unit.hpp>

UNITTEST(inistan) {
	sweet::Fector<int,100> f;
	AS_T(f.empty());
	f.push_back(1);
	AS_F(f.empty());
	AS_EQ(f.size(), 1);

	f.emplace_back(2);
	AS_F(f.empty());
	AS_EQ(f.size(), 2);
}

int main() {
	auto t(sweet::Unit::runTests());
	return t;
}
