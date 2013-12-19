/* dbc.hpp - a small but elegant Desing by Contract framework for C++ 
Author: Robert "burner" Schadek rburners@gmail.com License: LGPL 3 or higher
*/

#ifndef SWEET_DBC_HPP
#define SWEET_DBC_HPP
#include <type_traits>
#include <cmath>
#include <sstream>
#include <iomanip>
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
#ifndef SWEET_NO_DBC
#define RN(l,o,h) 	sweet::makeRange(l,o,h,#o)
#define NaN(v) 		sweet::makeNaN(v,#v)
#define NN(v) 		sweet::makeNulltest(v,#v)
#define NE(v) 		sweet::makeEmptytest(v,#v)
#define SB(v,s) 	sweet::makeSizetest(v,s,#v,#s)
#define TE(v) 		sweet::makeTruetest(v,#v)
#define GT(v,l)		sweet::makeGreaterThantest(v,l,#v,#l)
#define SE(v,l)		sweet::makeSmallerEqualtest(v,l,#v,#l)
#define EQ(v,l)		sweet::makeEqualtest(v,l,#v,#l)
#else
#define RN(l,o,h) sweet::makeDummy(o)
#define NaN(v)	 sweet::makeDummy(v)
#define NN(v) sweet::makeDummy(v) 
#define NE(v) sweet::makeDummy(v) 
#define SB(v,s) sweet::makeDummy(v) 
#define TE(v) sweet::makeDummy(v)
#define GT(v,l) sweet::makeDummy(v) 
#define SE(v,l) sweet::makeDummy(v) 
#define EQ(v,l) sweet::makeDummy(v) 
#endif

// Ensure macros
#define Esr(tests) sweet::testEnsure(__FILE__, __LINE__, tests)

// Main convience macros
#define Rqr(...) sweet::testCondition(__FILE__, __LINE__, __VA_ARGS__)

namespace sweet {

template<typename T>
struct BaseCls {
	T value;
	typedef T value_type;
	std::string name;
	inline BaseCls(T v, const std::string& n) : value(v), name(n) { }
};

template<typename T>
struct DummyTest : BaseCls<T> {
	inline DummyTest(T v) : BaseCls<T>(v, "") {}
	inline bool test() const { return true; }
	inline void msg(std::ostream&) {}
};

template<typename T>
inline DummyTest<T> makeDummy(T v) {
	return DummyTest<T>(v);
}


// Range test
template<typename T>
struct RangeTest : BaseCls<T> {
	T low;
	T high;

	inline RangeTest(T l, T o, T h, const std::string& n) : BaseCls<T>(o, n), 
		low(l), high(h) {}
	inline bool test() { return low <= this->value && this->value <= high; }
	inline void msg(std::ostream& s) {
		s<<std::fixed<<std::setprecision(7)<<'\t'<<"Range violation for \""<<
			this->name<<"\" "<<low<<" <= " <<this->value<<" <= "<<high;
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
	std::string sn;
	inline SizeTest(T v, size_t s, const std::string& n, const std::string& nn) : 
			BaseCls<T>(v, n), size(s), sn(nn) { }
	inline bool test() { return this->value.size() >= size; }
	inline void msg(std::ostream& s) {
		s<<'\t'<<"Size of value \""<<this->name<<"\" is smaller than \""<<sn<<"("
		<<size<<")\"";
	}
};

template<typename T>
inline SizeTest<T> makeSizetest(T v, size_t s, const std::string& n, const std::string& sn) {
	return SizeTest<T>(v,s,n,sn);
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


// SmallerEqual test
template<typename T>
struct SmallerEqualTest : BaseCls<T> {
	T lowerBound;
	std::string sme;
	inline SmallerEqualTest(T v, T lower, const std::string& n, const std::string& se) : 
		BaseCls<T>(v, n), lowerBound(lower), sme(se) {}
	inline bool test() { return this->value <= lowerBound; }
	inline void msg(std::ostream& s) {
		s<<"\t\""<<this->name<<"("<<this->value<<")\" was not greater than \""<<sme<<"("<<lowerBound<<")\"";
	}
};

template<typename T, typename S>
inline SmallerEqualTest<T> makeSmallerEqualtest(T v, S l, const std::string& n, const std::string& o) {
	return SmallerEqualTest<T>(v,l,n,o);
}


// GreaterThan test
template<typename T>
struct GreaterThanTest : BaseCls<T> {
	T lowerBound;
	std::string grt;
	inline GreaterThanTest(T v, T lower, const std::string& n, const std::string& gt) : 
		BaseCls<T>(v, n), lowerBound(lower), grt(gt) {}
	inline bool test() { return this->value > lowerBound; }
	inline void msg(std::ostream& s) {
		s<<"\t\""<<this->name<<"("<<this->value<<")\" was not greater than \""<<grt<<"("<<lowerBound<<")\"";
	}
};

template<typename T, typename S>
inline GreaterThanTest<T> makeGreaterThantest(T v, S l, const std::string& n, const std::string& o) {
	return GreaterThanTest<T>(v,l,n,o);
}


// Equal test
template<typename T>
struct EqualTest : BaseCls<T> {
	T l;
	T r;
	std::string other;
	inline EqualTest(T ll, T rr, const std::string& n, const std::string& o) : 
		BaseCls<T>(ll, n), l(ll), r(rr), other(o) {}
	inline bool test() { return this->l == this->r; }
	inline void msg(std::ostream& s) {
		s<<"\t\""<<this->l<<"\" was not equal \""<<other<<"("<<this->r<<")\"";
	}
};

template<typename T, typename S>
inline EqualTest<T> makeEqualtest(T v, S l, const std::string& n, const std::string& o) {
	return EqualTest<T>(v,l,n,o);
}


// Require check
template<typename S>
inline bool testConditionImplImpl(S s) {
	bool passed = false;
	bool excp = false;
#ifndef SWEET_NO_DBC
	try {
		passed = s.test();
	} catch(...) {
		excp = true;
	}
#endif
	return passed || excp;
}

template<typename S>
#ifndef SWEET_NO_DBC
inline bool testConditionImpl(std::ostream& ss, const char* file, int line, S s) {
#else
inline bool testConditionImpl(std::ostream& , const char* , int , S ) {
#endif
#ifndef SWEET_NO_DBC
	bool testRslt = testConditionImplImpl(s);
	if(!testRslt) {
		ss<<"At File '"<<file<<":"<<line<<" ";
		s.msg(ss);
		ss<<std::endl;
	}
	return testRslt;
#else
	return true;
#endif
} 

template<typename S,typename...Ts>
#ifndef SWEET_NO_DBC
inline bool testConditionImpl(std::ostream& ss, const char* file, int line, S s, Ts... t) {
#else
inline bool testConditionImpl(std::ostream& , const char* , int , S , Ts... ) {
#endif
#ifndef SWEET_NO_DBC
	bool testRslt = testConditionImplImpl(s);
	if(!testRslt) {
		ss<<"At File '"<<file<<":"<<line<<" ";
		s.msg(ss);
		ss<<std::endl;
	}
	bool other = testConditionImpl(ss, file, line, t...);
	return other && testRslt;
#else
	return true;
#endif
} 

template<typename... Ts>
#ifdef SWEET_NO_DBC
inline void testCondition(const char* , int , Ts...) {
#else
inline void testCondition(const char* file, int line, Ts... t) {
	//std::stringstream s;
	bool passed = testConditionImpl(std::cerr,file,line,t...);
	if(!passed) {
		std::cerr<<"REQUIRMENT TEST IN "<<file<<':'<<line<<" FAILED"<<std::endl;
		exit(1);
	}
#endif
} 


// Ensure
template<typename T>
#ifdef SWEET_NO_DBC
inline typename T::value_type testEnsure(const char* , int , T t) {
#else
inline typename T::value_type testEnsure(const char* file, int line, T t) {
	bool passed = testConditionImpl(std::cerr,file,line,t);
	if(!passed) {
		std::cerr<<"ENSURANCE TEST IN "<<file<<':'<<line<<" FAILED"<<std::endl;
		exit(1);
	}
#endif
	return t.value;
} 

template<typename T>
#ifdef SWEET_NO_DBC
inline bool testEnsureB(const char* , int line, T) {
#else
inline bool testEnsureB(const char* file, int line, T t) {
	bool passed = testConditionImpl(std::cerr,file,line,t);
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
#ifndef SWEET_NO_DBC
		(*this)(true);
#endif
	}

#ifdef SWEET_NO_DBC
	inline void operator()(bool = false) {
#else
	inline void operator()(bool after = false) {
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
