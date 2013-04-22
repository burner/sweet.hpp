#include <dbc.hpp>

void testFunc(int a, double d) {
	T(R(0,a,10), NaN(d), R(0.0,d,1.0));
}

int main() {
	return 0;
}
