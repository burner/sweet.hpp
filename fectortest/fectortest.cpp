#include <vector>

#include <fector.hpp>
#include <unit.hpp>

UNITTEST(vect) {
	std::vector<int> f;
	f.emplace_back(2);
}

UNITTEST(inistan) {
	sweet::Fector<int,100> f;
	AS_T(f.empty());
	f.push_back(1);
	AS_F(f.empty());
	AS_EQ(f.size(), 1);

	AS_EQ(f.front(), f.back());

	for(auto it : f) {
		AS_EQ(it, 1);		
	}

	const auto f2 = f;
	auto it = f2.begin();
	AS_EQ(*it, 1);		
}

int main() {
	auto t(sweet::Unit::runTests());
	return t;
}
