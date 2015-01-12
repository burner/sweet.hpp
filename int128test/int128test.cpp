#include <int128.hpp>
#include <unit.hpp>
#include <format.hpp>
#include <conv.hpp>

UNITTEST(modTest) {
	for(int i = 1; i < 1<<24; ++i) {
		for(int j = 1; j < 1<<24; ++j) {
			auto r(int128(i) % int128(j));
			AS_EQ_C(r, i % j, [&]() {
				format(std::cout, "%u %u int128(%s) int(%u)\n", i, j, r, i%j);
			});
		}
	}
}

int main() {
	int128 a(1337.0);
	int128 b(1338);
	int128 e(to<short>(1338));
	e *= to<uint16_t>(23);

	int128 c(a+b);
	ASSERT_EQ_C(c, 1337+1338, [&]() {
		format(std::cout, "%s", c);
	});

	ASSERT_EQ(a / int128(1337), 1);
	ASSERT_EQ(a * int128(2), 2674);
	ASSERT_EQ(a + int128(2), 1339);
	ASSERT_EQ(a + 2, 1339);
	ASSERT_EQ(a - 2, 1335);

	int128 d(2LU<<29);
	d *= (2LU<<29);
	format(std::cout, "%s\n", d);

	int128 f(1);
	for(int i = 1; i < 34; ++i) {
		f *= i;	
		//format(std::cout, "%22u, %22u\n", f.high, f.low);
	}

	auto t(sweet::Unit::runTests());

	return 0;
}
