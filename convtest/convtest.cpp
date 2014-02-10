#include <conv.hpp>
#include <unit.hpp>

template<typename T>
bool isStringFunc(const T& t) {
	return std::is_same<std::string, T>::value || std::is_same<const char*, T>::value ||
		(std::is_array<T>::value && std::is_same<char, typename std::remove_extent<T>::type>::value);
}

UNITTEST(traits) {
	AS_T((std::is_assignable<std::string, char[4]>::value));
	AS_T((std::is_assignable<std::string, int>::value));
	AS_T(isStringFunc("foo"));
	AS_T(is_string<std::string>::value);
}

UNITTEST(toString) {
	AS_EQ("1337", to<std::string>(1337));
	AS_EQ("1337.000000", to<std::string>(1337.0));
	AS_EQ("foo", to<std::string>("foo"));
	AS_EQ(1337, to<int>("1337"));
	AS_EQ(1337.000000, to<double>("1337.0"));
}

UNITTEST(intConvTest) {
	short s = 1;
	char c = to<char>(s);
	AS_EQ(c, 1);
}

int main() {
	return sweet::Unit::runTests();
}
