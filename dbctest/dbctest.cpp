#include <memory>
#include <dbc.hpp>
#include <logger.hpp>

class InvarTest {
public:
	int a = 0;
	int b = 9;

	INVARIANT_BEGIN
		Inv(R(0,a,32));
		Inv(R(0,b,10));
	INVARIANT_END

	inline void changeMethod() {
		Invariant();
		a = 33; 	// this contradicts the invariant
	}
};

void testFunc(int a, double d, int* ip) {
	Rqr(R(0,a,10), NaN(d), R(0.0,d,1.0), N(ip));
}

void testFunc2(std::vector<int>& a, std::shared_ptr<int> sp) {
	Rqr( S(a,0), T(a.size() % 12 == 0), N(sp));
}

int main() {
	int a;
	int b = Esr(R(8,a,10));
	std::vector<int> v;
	std::shared_ptr<int> sp(NULL);
	InvarTest ivt;
	testFunc(Esr(R(10,b,12)), 0.7, &a);
	ivt.changeMethod();
	testFunc(9, 0.1, &a);
	testFunc2(v, sp);
	return 0;
}
