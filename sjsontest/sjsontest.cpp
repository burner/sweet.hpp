#include <iostream>
#include <sjson.hpp>
#include <unit.hpp>

UNITTEST(sjson_test1) {
	sweet::jsonparser t("../sjsontest/hull.js");
	auto r = t.getRoot();
	AS_EQ(r->access("Config.system.numNodes")->getInt(), 32);
}

UNITTEST(sjson_test2) {
	sweet::jsonparser t2("../sjsontest/hull2.js");
	auto r = t2.getRoot();
	std::cout<<t2;
}
