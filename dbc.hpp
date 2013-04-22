#ifndef SWEET_DBC_HPP
#define SWEET_DBC_HPP
#include <type_traits>
#include <cmath>
#include <sstream>

// Range test
template<typename T>
struct Range {
	T low;
	T obj;
	T high;
	std::string name;

	inline Range(T l, T o, T h, const std::string& n) : low(l), obj(o), high(h), name(n) {}
	inline bool test() { return low <= obj && obj <= high; }
	inline void msg(std::stringstream& s) {
		s<<"Range violation for \""<<name<<"\" "<<low<<" <= "<<obj<<" <= "<<high;
	}
};

template<typename T>
inline Range<T> makeRange(T l, T o, T h, const std::string& n) {
	return Range<T>(l,o,h,n);
}

#define R(l,o,h) makeRange(l,o,h,#o)


// NaN test

template<class T, class Enable = void>
class NaNclass; // undefined

template<typename T>
struct NaNclass<T, typename std::enable_if<std::is_floating_point<T>::value >::type> {
	T value;
	std::string name;
	inline NaNclass(T v, const std::string& n) : value(v), name(n) { }
	inline bool test() { return !std::isnan(value); }
	inline void msg(std::stringstream& s) {
		s<<"Value is NaN";
	}
};

template<typename T>
inline NaNclass<T> makeNaN(T v, const std::string& n) {
	return NaNclass<T>(v,n);
}

#define NaN(v) makeNaN(v,#v)


// Argument check

template<typename S>
inline void testConditionImpl(std::stringstream& ss, S s) {

} 

template<typename S,typename...Ts>
inline void testConditionImpl(std::stringstream& ss, S s,Ts... t) {

	testConditionImpl(ss, t...);

} 

template<typename... Ts>
inline void testCondition(const char* File, int line, Ts... t) {
	std::stringstream s;
	testConditionImpl(s,t...);

} 

#define T(tests...) testCondition(__FILE__, __LINE__, tests)
#endif
