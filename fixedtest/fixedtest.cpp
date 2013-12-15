#include <fixed.hpp>
#include <unit.hpp>

using namespace sweet;

UNITTEST(decimal1) {
	Fixed d(1);

	std::cout<<std::fixed<<d<<std::endl;
}

int main() {
	Unit::runTests();
	return 0;
}

UNITTEST(decimalTest1) {
	std::vector<Fixed> nums = {
		Fixed(889.122),
		Fixed(858.580),
		Fixed(234.366),
		Fixed(364.329),
		Fixed(957.409),
		Fixed(398.172),
		Fixed(961.347),
		Fixed(799.327),
		Fixed(754.603),
		Fixed(864.419),
		Fixed(184.259),
		Fixed(96.347),
		Fixed(475.833),
		Fixed(507.306),
		Fixed(285.686),
		Fixed(299.183),
		Fixed(732.102),
		Fixed(549.708),
		Fixed(302.171),
		Fixed(341.213),
		Fixed(656.841),
		Fixed(868.370),
		Fixed(723.710),
		Fixed(897.054),
		Fixed(891.672),
	
		Fixed("889.122"),
		Fixed("858.580"),
		Fixed("234.366"),
		Fixed("364.329"),
		Fixed("957.409"),
		Fixed("398.172"),
		Fixed("961.347"),
		Fixed("799.327"),
		Fixed("754.603"),
		Fixed("864.419"),
		Fixed("184.259"),
		Fixed("96.347"),
		Fixed("475.833"),
		Fixed("507.306"),
		Fixed("285.686"),
		Fixed("299.183"),
		Fixed("732.102"),
		Fixed("549.708"),
		Fixed("302.171"),
		Fixed("341.213"),
		Fixed("656.841"),
		Fixed("868.370"),
		Fixed("723.710"),
		Fixed("897.054"),
		Fixed("891.672")
	};

	for(auto& it : nums) {
		//double dv = it;
		std::cout<<std::setprecision(10)<<it<<std::endl;
	}
}
