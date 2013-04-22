#ifndef SWEET_DBC_HPP
#define SWEET_DBC_HPP
#include <type_traits>
#include <cmath>
#include <sstream>
#include <ostream>
#include <iostream>
//#include <stdexcept>
#include <stdlib.h>

// Range test
template<typename T>
struct RangeTest {
	T low;
	T value;
	T high;
	std::string name;
	typedef T value_type;

	inline RangeTest(T l, T o, T h, const std::string& n) : low(l), value(o), high(h), name(n) {}
	inline bool test() { return low <= value && value <= high; }
	inline void msg(std::ostream& s) {
		s<<"Range violation for \""<<name<<"\" "<<low<<" <= "<<value<<" <= "<<high;
	}
};

template<typename T>
inline RangeTest<T> makeRange(T l, T o, T h, const std::string& n) {
	return RangeTest<T>(l,o,h,n);
}

#define R(l,o,h) makeRange(l,o,h,#o)


// NaN test
template<class T, class Enable = void>
class NaNclassTest; // undefined

template<typename T>
struct NaNclassTest<T, typename std::enable_if<std::is_floating_point<T>::value >::type> {
	T value;
	typedef T value_type;
	std::string name;
	inline NaNclassTest(T v, const std::string& n) : value(v), name(n) { }
	inline bool test() { return !std::isnan(value); }
	inline void msg(std::ostream& s) {
		s<<"Value of \""<<name<<"\" is NaN";
	}
};

template<typename T>
inline NaNclassTest<T> makeNaN(T v, const std::string& n) {
	return NaNclassTest<T>(v,n);
}

#define NaN(v) makeNaN(v,#v)


// Null test
template<typename T>
struct NullTest {
	T value;
	typedef T value_type;
	std::string name;
	inline NullTest(T v, const std::string& n) : value(v), name(n) { }
	inline bool test() { return value != nullptr; }
	inline void msg(std::ostream& s) {
		s<<"Value \""<<name<<"\" is null";
	}
};

template<typename T>
inline NullTest<T> makeNulltest(T v, const std::string& n) {
	return NullTest<T>(v,n);
}

#define N(v) makeNulltest(v,#v)


// Require check
template<typename S>
inline bool testConditionImplImpl(S s) {
	bool passed = false;
	bool excp = false;
	try {
		passed = s.test();
	} catch(...) {
		excp = true;
	}
	return passed || excp;
}

template<typename S>
inline bool testConditionImpl(std::ostream& ss, S s) {
	bool testRslt = testConditionImplImpl(s);
	if(!testRslt) {
		s.msg(ss);
		ss<<std::endl;
	}
	return testRslt;
} 

template<typename S,typename...Ts>
inline bool testConditionImpl(std::ostream& ss, S s, Ts... t) {
	bool testRslt = testConditionImplImpl(s);
	if(!testRslt) {
		s.msg(ss);
		ss<<std::endl;
	}
	bool other = testConditionImpl(ss, t...);
	return other && testRslt;

} 

template<typename... Ts>
inline void testCondition(const char* File, int line, Ts... t) {
#ifdef DBC_RELEASE
#else
	//std::stringstream s;
	bool passed = testConditionImpl(std::cerr,t...);
	if(!passed) {
		std::cerr<<"REQUIRMENT TEST IN "<<File<<':'<<line<<" FAILED"<<std::endl;
		exit(1);
	}
#endif
} 

#define Rqr(tests...) testCondition(__FILE__, __LINE__, tests)


// Ensure
template<typename T>
inline typename T::value_type testEnsure(const char* File, int line, T t) {
#ifndef DBC_RELEASE
	//std::stringstream s;
	bool passed = testConditionImpl(std::cerr,t);
	if(!passed) {
		std::cerr<<"ENSURANCE TEST IN "<<File<<':'<<line<<" FAILED"<<std::endl;
		exit(1);
	}
#endif

	return t.value;
} 

#define Esr(tests) testEnsure(__FILE__, __LINE__, tests)
#endif
