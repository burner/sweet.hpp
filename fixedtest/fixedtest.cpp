#include <random>
#include <fixed.hpp>
#include <unit.hpp>

using namespace sweet;

UNITTEST(decimal1) {
	Fixed d(1);

	std::cout<<std::fixed<<d<<std::endl;
}

UNITTEST(decimal2) {
	Fixed d("1.5");
	d*=4;
	d*=4.0;

	d-=4;
	d-=4.0;

	d/=4;
	d/=4.0;

	d+=4;
	d+=4.0;

	std::cout<<std::fixed<<d<<std::endl;
}

UNITTEST(randomGen) {
	std::random_device rd;
	std::mt19937 m(rd());
	std::uniform_real_distribution<> dist(
		std::numeric_limits<int32_t>::min()/2,
		std::numeric_limits<int32_t>::max()/2
	);

	/*for(size_t i = 0; i < std::numeric_limits<size_t>::max() / 100000; ++i) {
		double a(dist(m));
		double b(dist(m));

		AS_EQ(Fixed(std::to_string(a)) + Fixed(std::to_string(b)),
			Fixed(std::to_string(a+b)));

		AS_EQ(Fixed(std::to_string(a)) - Fixed(std::to_string(b)),
			Fixed(std::to_string(a-b)));

		AS_EQ(Fixed(std::to_string(a)) * Fixed(std::to_string(b)),
			Fixed(std::to_string(a*b)));

		AS_EQ(Fixed(std::to_string(a)) / Fixed(std::to_string(b)),
			Fixed(std::to_string(a/b)));

		AS_EQ(Fixed(std::to_string(a)) < Fixed(std::to_string(b)),
			a < b);
		AS_EQ(Fixed(std::to_string(a)) > Fixed(std::to_string(b)),
			a > b);
	}
	*/
}
