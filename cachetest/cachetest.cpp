#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include <logger.hpp>
#include <unit.hpp>
#include <cache.hpp>

UNITTEST(traitstest) {
	std::vector<int> v1;
	std::list<int> l1;
	std::set<int> s1;
	std::map<int,int> m1;
	std::unordered_map<int,int> um1;
	std::unordered_set<int> us1;
	LOG("Size of empty int vector %u", sweet::SizeOf(v1));
	LOG("Size of empty int list %u", sweet::SizeOf(l1));
	LOG("Size of empty int set %u", sweet::SizeOf(s1));
	LOG("Size of empty int int map %u", sweet::SizeOf(m1));
	LOG("Size of empty int int unordered_map %u", sweet::SizeOf(um1));
	LOG("Size of empty int unordered_set %u", sweet::SizeOf(us1));
	AS_T(sweet::is_associated<std::set<int>>::value);
	AS_T(sweet::is_associated<std::unordered_set<int>>::value);
	AS_T((sweet::is_associated<std::unordered_map<int,int>>::value));
	AS_T((sweet::is_associated<std::map<int,int>>::value));

}

int main() {
	sweet::Unit::runTests();
	std::cout<<"Number of Asserts "<<
		sweet::Unit::getNumOfAsserts()<<std::endl;
}
