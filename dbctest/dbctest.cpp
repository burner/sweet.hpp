#include <memory>
#include <dbc.hpp>
#include <logger.hpp>
#include <unit.hpp>

class InvarTest {
public:
	int a = 0;
	int b = 9;

	INVARIANT_BEGIN
		Inv(RN(0,a,32));
		Inv(RN(0,b,10));
	INVARIANT_END

	inline void changeMethod() {
		Invariant();
		a = 10; 	// this contradicts does not the invariant
	}
};

void testFunc(int a, double d, int* ip) {
	Rqr(RN(0,a,10), NaN(d), RN(0.0,d,1.0), NN(ip));
}

void testFunc2(std::vector<int>& a, std::shared_ptr<int> sp) {
	Rqr( SB(a,0), TE(a.size() % 12 == 0), NN(sp));
}

UNITTEST(dbctest) {
	int a = 9;
	int b = Esr(RN(8,a,10));
	std::vector<int> v;
	//std::shared_ptr<int> sp(NULL);
	InvarTest ivt;
	testFunc(Esr(RN(7,b,12)), 0.7, &a);
	ivt.changeMethod();
	testFunc(9, 0.1, &a);
	//testFunc2(v, sp);
}
