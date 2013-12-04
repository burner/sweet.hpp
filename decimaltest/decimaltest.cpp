#include <decimal.hpp>

int main() {
	Decimal d;

	auto d1(d + 1);
	auto d2(d + 1.0);

	d1 += 2;
	return 0;
}
