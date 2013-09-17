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

#define Invariant auto fancyNameNobodyWillEverGuess( \
sweet::makeInvariantStruct(this, __FILE__, __LINE__)); \
fancyNameNobodyWillEverGuess

// Test macros
#define RN(l,o,h) 	sweet::makeRange(l,o,h,#o)
#define NaN(v) 		sweet::makeNaN(v,#v)
#define NN(v) 		sweet::makeNulltest(v,#v)
#define NE(v) 		sweet::makeEmptytest(v,#v)
#define SB(v,s) 	sweet::makeSizetest(v,s,#v)
#define TE(v) 		sweet::makeTruetest(v,#v)
#define GT(v,l)		sweet::makeGreaterThantest(v,l,#v)

// Ensure macros
#define Esr(tests) sweet::testEnsure(__FILE__, __LINE__, tests)

// Main convience macros
#define Rqr(tests...) sweet::testCondition(__FILE__, __LINE__, tests)

namespace sweet {

template<typename T>
struct BaseCls {
	T value;
	typedef T value_type;
	std::string name;
	inline BaseCls(T v, const std::string& n) : value(v), name(n) { }
};


// Range test
template<typename T>
struct RangeTest : BaseCls<T> {
	T low;
	T high;

	inline RangeTest(T l, T o, T h, const std::string& n) : BaseCls<T>(o, n), 
		low(l), high(h) {}
	inline bool test() { return low <= this->value && this->value <= high; }
	inline void msg(std::ostream& s) {
		s<<'\t'<<"Range violation for \""<<this->name<<"\" "<<low<<" <= "
			<<this->value<<" <= "<<high;
	}
};

template<typename T, typename S, typename R>
inline RangeTest<S> makeRange(T l, S o, R h, const std::string& n) {
	return RangeTest<S>(static_cast<S>(l),o,static_cast<S>(h),n);
}


// NaN test
template<class T, class Enable = void>
struct NaNclassTest; // undefined

template<typename T>
struct NaNclassTest<T, 
		typename std::enable_if<std::is_floating_point<T>::value >::type>
		: public BaseCls<T> {
	inline NaNclassTest(T v, const std::string& n) : BaseCls<T>(v,n) { }
	inline bool test() { return !std::isnan(this->value); }
	inline void msg(std::ostream& s) {
		s<<'\t'<<"Value of \""<<this->name<<"\" is NaN";
	}
};

template<typename T>
inline NaNclassTest<T> makeNaN(T v, const std::string& n) {
	return NaNclassTest<T>(v,n);
}


// Null test
template<typename T>
struct NullTest : BaseCls<T> {
	inline NullTest(T v, const std::string& n) : BaseCls<T>(v, n) { }
	inline bool test() { return this->value != nullptr; }
	inline void msg(std::ostream& s) {
		s<<'\t'<<"Value \""<<this->name<<"\" is null";
	}
};

template<typename T>
inline NullTest<T> makeNulltest(T v, const std::string& n) {
	return NullTest<T>(v,n);
}


// Empty test
template<typename T>
struct EmptyTest : BaseCls<T> {
	inline EmptyTest(T v, const std::string& n) : BaseCls<T>(v, n) { }
	inline bool test() { return !this->value.empty(); }
	inline void msg(std::ostream& s) {
		s<<'\t'<<"Value \""<<this->name<<"\" is empty";
	}
};

template<typename T>
inline EmptyTest<T> makeEmptytest(T v, const std::string& n) {
	return EmptyTest<T>(v,n);
}


// Size test
template<typename T>
struct SizeTest : BaseCls<T> {
	size_t size;
	inline SizeTest(T v, size_t s, const std::string& n) : BaseCls<T>(v, n),
			size(s) { }
	inline bool test() { return this->value.size() >= size; }
	inline void msg(std::ostream& s) {
		s<<'\t'<<"Size of value \""<<this->name<<"\" is smaller than "<<size;
	}
};

template<typename T>
inline SizeTest<T> makeSizetest(T v, size_t s, const std::string& n) {
	return SizeTest<T>(v,s,n);
}


// True test
struct TrueTest : BaseCls<bool> {
	inline TrueTest(bool v, const std::string& n) : BaseCls<bool>(v, n) {}
	inline bool test() { return this->value; }
	inline void msg(std::ostream& s) {
		s<<'\t'<<"Expression \""<<this->name<<"\" was not true";
	}
};

inline TrueTest makeTruetest(bool v, const std::string& n) {
	return TrueTest(v,n);
}


// GreaterThan test
template<typename T>
struct GreaterThanTest : BaseCls<T> {
	T lowerBound;
	inline GreaterThanTest(T v, T lower, const std::string& n) : BaseCls<T>(v, n), 
		lowerBound(lower) {}
	inline bool test() { return this->value > lowerBound; }
	inline void msg(std::ostream& s) {
		s<<"\t\""<<this->name<<"\" was not greater than \""<<lowerBound<<"\"";
	}
};

template<typename T, typename S>
inline GreaterThanTest<T> makeGreaterThantest(T v, S l, const std::string& n) {
	return GreaterThanTest<T>(v,l,n);
}


// Require check
template<typename S>
inline bool testConditionImplImpl(S s) {
	bool passed = false;
	bool excp = false;
#ifndef DBC_RELEASE
	try {
		passed = s.test();
	} catch(...) {
		excp = true;
	}
#endif
	return passed || excp;
}

template<typename S>
inline bool testConditionImpl(std::ostream& ss, S s) {
#ifndef DBC_RELEASE
	bool testRslt = testConditionImplImpl(s);
	if(!testRslt) {
		s.msg(ss);
		ss<<std::endl;
	}
	return testRslt;
#else
	return true;
#endif
} 

template<typename S,typename...Ts>
inline bool testConditionImpl(std::ostream& ss, S s, Ts... t) {
#ifndef DBC_RELEASE
	bool testRslt = testConditionImplImpl(s);
	if(!testRslt) {
		s.msg(ss);
		ss<<std::endl;
	}
	bool other = testConditionImpl(ss, t...);
	return other && testRslt;
#else
	return true;
#endif
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
inline typename T::value_type testEnsure(const char* File, int line, T t) {
#ifndef DBC_RELEASE
	bool passed = testConditionImpl(std::cerr,t);
	if(!passed) {
		std::cerr<<"ENSURANCE TEST IN "<<File<<':'<<line<<" FAILED"<<std::endl;
		exit(1);
	}
#endif
	return t.value;
} 

template<typename T>
inline bool testEnsureB(const char* File, int line, T t) {
#ifndef DBC_RELEASE
	bool passed = testConditionImpl(std::cerr,t);
	if(!passed) {
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

			std::cerr<<"INVARAIANT OF CLASS \""<<demangled<<"\" FAILED FROM "
				<<file<<':'<<line;
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
