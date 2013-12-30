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

UNITTEST(cachetest2) {
	sweet::cache<std::string,std::vector<int>, 48> c1;
	c1.insert("key1", std::vector<int>());
	c1.insert("key2", std::vector<int>());
	AS_T(c1.contains("key1"));
	AS_T(c1.contains("key2"));
	LOG("%u", c1.bytesStored());
	c1.insert("key3", std::vector<int>());
	AS_T(c1.contains("key1"));
	AS_T(c1.contains("key3"));
	AS_F(c1.contains("key2"));
	LOG("%u", c1.bytesStored());
}

UNITTEST(cachetest1) {
	sweet::cache<std::string,std::vector<int>, 4096> c1;
	c1.insert("key", std::vector<int>());
	AS_EQ(c1.size(), 1u);
	AS_T(c1.contains("key"));
	AS_F(c1.find("key") == c1.end());
	AS_T(c1.find("not_a_key") == c1.end());
	AS_F(c1.contains("not_a_key"));
	LOG("bytes stored %u", c1.bytesStored());
}

int main() {
	sweet::Unit::runTests();
	std::cout<<"Number of Asserts "<<
		sweet::Unit::getNumOfAsserts()<<std::endl;
}
