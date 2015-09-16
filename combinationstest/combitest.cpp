#include <combinations.hpp>

#include <iostream>
#include <algorithm>
#include <iterator>
#include <set>
#include <string>
#include <unit.hpp>

UNITTEST(testcombinations) {
	sweet::Combinations<> cs(10, 4);
	std::set<std::string> cmp;

    while (!cs.completed) {
		auto c = cs.next();
		std::string tmp;

		for(const auto& it : c) {
			tmp.append(std::to_string(it));
		}

		const auto i = cmp.insert(tmp);
		AS_T(i.second);
    }	

	AS_T(cmp.size() > 10);
}

#ifdef LOCAL
int main(int,char**) {
	return sweet::Unit::runTests();
}
#endif
