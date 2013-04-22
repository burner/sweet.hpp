#include <dbc.hpp>
#include <logger.hpp>

void testFunc(int a, double d, int* ip) {
	Rqr(R(0,a,10), NaN(d), R(0.0,d,1.0), N(ip));
}

int main() {
	int a;
	int b = 11;
	testFunc(9, 0.1, &a);
	testFunc(Esr(R(9,b,12)), 1.1, NULL);
	return 0;
}
