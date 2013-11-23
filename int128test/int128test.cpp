#include <int128.hpp>
#include <unit.hpp>
#include <format.hpp>

int main() {
	int128 a(1337.0);
	int128 b(1338);

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
	return 0;
}
