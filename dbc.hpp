/* unit.hpp - a small but elegant Desing by Contract framework for C++ 
Author: Robert "burner" Schadek rburners@gmail.com License: LGPL 3 or higher
*/

#ifndef SWEET_DBC_HPP
#define SWEET_DBC_HPP
#include <type_traits>
#include <cmath>
#include <sstream>
#include <ostream>
#include <iostream>
#include <stdlib.h>
#include <cxxabi.h>


// Invariant macros
#define INVARIANT_BEGIN bool InvariantTestMethod() { \
	bool InvarReturn = true;

#define INVARIANT_END return InvarReturn; }

#define Inv(tests) InvarReturn &= sweet::testEnsureB(__FILE__, __LINE__, tests)

#define Invariant auto fancyNameNobodyWillEverGuess(sweet::makeInvariantStruct(this, __FILE__, __LINE__)); \
fancyNameNobodyWillEverGuess



// Test macros
#define R(l,o,h) sweet::makeRange(l,o,h,#o)
#define NaN(v) sweet::makeNaN(v,#v)
#define N(v) sweet::makeNulltest(v,#v)
#define E(v) sweet::makeEmptytest(v,#v)
#define S(v,s) sweet::makeSizetest(v,s,#v)
#define T(v) sweet::makeTruetest(v,#v)
#define Esr(tests) sweet::testEnsureB(__FILE__, __LINE__, tests)

// Main convience macros
#define Rqr(tests...) sweet::testCondition(__FILE__, __LINE__, tests)

namespace sweet {
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
		s<<'\t'<<"Range violation for \""<<name<<"\" "<<low<<" <= "<<value<<" <= "<<high;
	}
};

template<typename T>
inline RangeTest<T> makeRange(T l, T o, T h, const std::string& n) {
	return RangeTest<T>(l,o,h,n);
}


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
		s<<'\t'<<"Value of \""<<name<<"\" is NaN";
	}
};

template<typename T>
inline NaNclassTest<T> makeNaN(T v, const std::string& n) {
	return NaNclassTest<T>(v,n);
}


// Null test
template<typename T>
struct NullTest {
	T value;
	typedef T value_type;
	std::string name;
	inline NullTest(T v, const std::string& n) : value(v), name(n) { }
	inline bool test() { return value != nullptr; }
	inline void msg(std::ostream& s) {
		s<<'\t'<<"Value \""<<name<<"\" is null";
	}
};

template<typename T>
inline NullTest<T> makeNulltest(T v, const std::string& n) {
	return NullTest<T>(v,n);
}


// Empty test
template<typename T>
struct EmptyTest {
	T value;
	typedef T value_type;
	std::string name;
	inline EmptyTest(T v, const std::string& n) : value(v), name(n) { }
	inline bool test() { return !value.empty(); }
	inline void msg(std::ostream& s) {
		s<<'\t'<<"Value \""<<name<<"\" is empty";
	}
};

template<typename T>
inline EmptyTest<T> makeEmptytest(T v, const std::string& n) {
	return EmptyTest<T>(v,n);
}


// Size test
template<typename T>
struct SizeTest {
	T value;
	typedef T value_type;
	std::string name;
	size_t size;
	inline SizeTest(T v, size_t s, const std::string& n) : value(v), name(n), 
			size(s) { }
	inline bool test() { return value.size() >= size; }
	inline void msg(std::ostream& s) {
		s<<'\t'<<"Size of value \""<<name<<"\" is smaller than "<<size;
	}
};

template<typename T>
inline SizeTest<T> makeSizetest(T v, size_t s, const std::string& n) {
	return SizeTest<T>(v,s,n);
}


// True test
struct TrueTest {
	bool value;
	typedef bool value_type;
	std::string name;
	size_t size;
	inline TrueTest(bool v, const std::string& n) : value(v), name(n) {}
	inline bool test() { return value; }
	inline void msg(std::ostream& s) {
		s<<'\t'<<"Expression \""<<name<<"\" was not true";
	}
};

inline TrueTest makeTruetest(bool v, const std::string& n) {
	return TrueTest(v,n);
}



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



// Ensure
template<typename T>
inline typename T::value_type testEnsure(const char* File, int line, T t, 
		bool die) {
#ifndef DBC_RELEASE
	bool passed = testConditionImpl(std::cerr,t);
	if(!passed) {
		std::cerr<<"ENSURANCE TEST IN "<<File<<':'<<line<<" FAILED"<<std::endl;
		if(die) {
			exit(1);
		}
	}
#endif
	return t.value;
} 

template<typename T>
inline bool testEnsureB(const char* File, int line, T t) {
#ifndef DBC_RELEASE
	bool passed = testConditionImpl(std::cerr,t);
	if(!passed) {
		//os<<"ENSURANCE TEST IN "<<File<<':'<<line<<" FAILED"<<std::endl;
		return false;
	}
#endif
	return true;
} 

template<typename T>
struct InvariantStruct {
	T cls;
	const char* file;
	int line;
	inline InvariantStruct(T c, const char* f, int l) : cls(c), file(f), 
			line(l) {
	}

	inline ~InvariantStruct() {
#ifndef DBC_RELEASE
		(*this)(true);
#endif
	}

	inline void operator()(bool after = false) {
#ifndef DBC_RELEASE
		bool rslt = cls->InvariantTestMethod();
		if(!rslt) {
			int status;
			char * demangled = abi::__cxa_demangle(
				typeid(typename std::remove_pointer<T>::type).name(),0,0,&status
			);

			std::cerr<<"INVARAIANT OF CLASS \""<<demangled<<"\" FAILED FROM "<<file
				<<':'<<line;
			std::cerr<<(after ? " AFTER THE METHOD" : " AT METHOD ENTRY");
			std::cerr<<std::endl;
			free(demangled);
			exit(1);
		}
#endif
	}
};

template<typename T>
InvariantStruct<T> makeInvariantStruct(T c, const char* f, int l) {
	return InvariantStruct<T>(c,f,l);
}

}
#endif
