#include <unit.hpp>
#include <compare.hpp>
#include <logger.hpp>

UNITTEST(intint) {
	AS_F(sweet::less(1337u,1337u));
	AS_T(sweet::lessEqual(1337u,1337u));
	AS_T(sweet::equal(1337u,1337u));
	AS_F(sweet::notEqual(1337u,1337u));
	AS_T(sweet::greaterEqual(1337u,1337u));
	AS_F(sweet::greater(1337u,1337u));

	AS_F(sweet::less(1337,1337u));
	AS_T(sweet::lessEqual(1337,1337u));
	AS_T(sweet::equal(1337,1337u));
	AS_F(sweet::notEqual(1337,1337u));
	AS_T(sweet::greaterEqual(1337,1337u));
	AS_F(sweet::greater(1337,1337u));

	AS_F(sweet::less(1337u,1337));
	AS_T(sweet::lessEqual(1337u,1337));
	AS_T(sweet::equal(1337u,1337));
	AS_F(sweet::notEqual(1337u,1337));
	AS_T(sweet::greaterEqual(1337u,1337));
	AS_F(sweet::greater(1337u,1337));

	AS_F(sweet::less(1337,1337));
	AS_T(sweet::lessEqual(1337,1337));
	AS_T(sweet::equal(1337,1337));
	AS_F(sweet::notEqual(1337,1337));
	AS_T(sweet::greaterEqual(1337,1337));
	AS_F(sweet::greater(1337,1337));

	AS_F(sweet::less(int128(1337),1337));
	AS_T(sweet::lessEqual(int128(1337),1337));
	AS_T(sweet::equal(int128(1337),1337));
	AS_F(sweet::notEqual(int128(1337),1337));
	AS_T(sweet::greaterEqual(int128(1337),1337));
	AS_F(sweet::greater(int128(1337),1337));

	AS_F(sweet::less(1337, int128(1337)));
	AS_T(sweet::lessEqual(1337, int128(1337)));
	AS_T(sweet::equal(1337, int128(1337)));
	AS_F(sweet::notEqual(1337, int128(1337)));
	AS_T(sweet::greaterEqual(1337, int128(1337)));
	AS_F(sweet::greater(1337, int128(1337)));

	AS_F(sweet::less(int128(1337), int128(1337)));
	AS_T(sweet::lessEqual(int128(1337), int128(1337)));
	AS_T(sweet::equal(int128(1337), int128(1337)));
	AS_F(sweet::notEqual(int128(1337), int128(1337)));
	AS_T(sweet::greaterEqual(int128(1337), int128(1337)));
	AS_F(sweet::greater(int128(1337), int128(1337)));
}

int main() {
	unsigned c = 0;
	if((c = sweet::Unit::runTests("TheNameOfTheFileWithTheBenchmarkResults"))) {
		WARN("%u errors occured", c);
	}
	return 0;
}
