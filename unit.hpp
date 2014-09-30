/* unit.hpp - a small but elegant unit test framework for C++ based on QUnit 
and Testdog

Author: Robert "burner" Schadek rburners@gmail.com License: LGPL 3 or higher

Example:
#include <fstream>
#include "unit.hpp"
	
UNITTEST(fancyname) {
	// std::ofstream unittestlog("testlog.tst"); 	optional
	// this->setOutputStream(&unittestlog); 		optional
	AS_T(true);
	AS_F(4!=4);
	AS_EQ(42, 42);
	AS_NEQ(42, 43);

	if(AS_T(true)) {
		// do a test that is only possible if the previous test passed
	}
}

UNITTEST(foo, 66) {
	AS_T(4==4);
}

UNITTEST(foon, 66666, "-O3") {
	AS_T(4==4);
}

int main() {
	if(!sweet::Unit::runTests(
			"TheOptionalNameOfTheFileWithTheBenchmarkResults")) {
		return 1;
	}
}
*/

#pragma once
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <cmath>
#include <type_traits>
#include <limits>
#include <iomanip>
#include <random>


// RANDOMIZED_TEST macros

#define VA_NARGS_IMPL(_1,_2,_3,_4,_5,_6,_7,_8,N,...) N
#define VA_NARGS(...) VA_NARGS_IMPL(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)
#define SET_STATEGENERATE(name, sweetGen, count, ...)   \
	[&](size_t numRuns) {							\
		auto __funcToCall = name;						\
		auto& __sweetGenerator = sweetGen;				\
        DEC ## count (__VA_ARGS__)                      \
		for(size_t cnt = 0; cnt < numRuns; ++cnt) {		\
        	__funcToCall(REF ## count (__VA_ARGS__));   \
		}												\
	}

#define SET_STATEP(name, sweetGen, count, ...) \
	SET_STATEGENERATE(name, sweetGen, count, __VA_ARGS__) 

#define RANDOMIZED_TEST(name, sweetGen, ...) \
	SET_STATEP(name, sweetGen, VA_NARGS(__VA_ARGS__), __VA_ARGS__)

/* args */
#define DEC1(a) auto __sweet_value_A = a;
#define DEC2(a,b) DEC1(a) auto __sweet_value_B = b;
#define DEC3(a,b,c) DEC2(a, b) auto __sweet_value_C = c;
#define DEC4(a,b,c,d) DEC3(a,b,c) auto __sweet_value_D = d;
#define DEC5(a,b,c,d,e) DEC4(a,b,c,d) auto __sweet_value_E = e;
#define DEC6(a,b,c,d,e,f) DEC5(a,b,c,d,e) auto __sweet_value_F = f;
#define DEC7(a,b,c,d,e,f,g) DEC6(a,b,c,d,e,f) auto __sweet_value_G = g;
#define DEC8(a,b,c,d,e,f,g,h) DEC7(a,b,c,d,e,f,g) auto __sweet_value_H = h;
#define REF1(a) __sweet_value_A(__sweetGenerator)
#define REF2(a,b) REF1(a), __sweet_value_B(__sweetGenerator)
#define REF3(a,b,c) REF2(a,b), __sweet_value_C(__sweetGenerator)
#define REF4(a,b,c,d) REF3(a,b,c), __sweet_value_D(__sweetGenerator)
#define REF5(a,b,c,d,e) REF4(a,b,c,d), __sweet_value_E(__sweetGenerator)
#define REF6(a,b,c,d,e,f) REF5(a,b,c,d,e), __sweet_value_F(__sweetGenerator)
#define REF7(a,b,c,d,e,f,g) REF6(a,b,c,d,e,f), __sweet_value_G(__sweetGenerator)
#define REF8(a,b,c,d,e,f,g, h) REF7(a,b,c,d,e,f,g), __sweet_value_H(__sweetGenerator)

// RANDOMIZED_TEST macros END

#define PP_HAS_ARGS_IMPL2(_1, _2, _3, N, ...) N
#define PP_HAS_ARGS_SOURCE() MULTI, MULTI, ONE, ERROR
#define PP_HAS_ARGS_IMPL(...) PP_HAS_ARGS_IMPL2(__VA_ARGS__)
#define PP_HAS_ARGS(...)      PP_HAS_ARGS_IMPL(__VA_ARGS__, \
PP_HAS_ARGS_SOURCE())

#define __UTEST_NOTEST_ONE(test_name) \
class test_name##_test_class : public sweet::Unit::Unittest { void nevercall(); \
public: \
test_name##_test_class() : sweet::Unit::Unittest(#test_name,__FILE__,__LINE__) {} \
} test_name##_test_class_impl; \
void test_name##_test_class::nevercall()

#define __UTEST_NOTEST_MULTI(test_name,...) \
class test_name##_test_class : public sweet::Unit::Unittest { void nevercall(); \
public: \
test_name##_test_class() : sweet::Unit::Unittest(#test_name,__FILE__,__LINE__,\
__VA_ARGS__) {}  } test_name##_test_class_impl; \
void test_name##_test_class::nevercall()

#define __UTEST_NOTEST_DISAMBIGUATE2(has_args, ...) __UTEST_NOTEST_ \
## has_args (__VA_ARGS__)
#define __UTEST_NOTEST_DISAMBIGUATE(has_args, ...) \
__UTEST_NOTEST_DISAMBIGUATE2(has_args, __VA_ARGS__)
#define __UTEST_NOTEST(...) __UTEST_NOTEST_DISAMBIGUATE(PP_HAS_ARGS(\
__VA_ARGS__), __VA_ARGS__)


#define __UTEST_ONE(test_name) \
class test_name##_test_class : public sweet::Unit::Unittest { void run_impl(); \
public: \
test_name##_test_class() : sweet::Unit::Unittest(#test_name,__FILE__,__LINE__) {} \
} test_name##_test_class_impl; \
void test_name##_test_class::run_impl()

#define __UTEST_MULTI(test_name,...) \
class test_name##_test_class : public sweet::Unit::Unittest { void run_impl(); \
public: \
test_name##_test_class() : sweet::Unit::Unittest(#test_name,__FILE__,__LINE__,\
__VA_ARGS__) {} } test_name##_test_class_impl; \
void test_name##_test_class::run_impl()

#define __UTEST_DISAMBIGUATE2(has_args, ...) __UTEST_ ## has_args (__VA_ARGS__)
#define __UTEST_DISAMBIGUATE(has_args, ...) __UTEST_DISAMBIGUATE2(has_args, \
__VA_ARGS__)
#define __UTEST(...) __UTEST_DISAMBIGUATE(PP_HAS_ARGS(__VA_ARGS__), __VA_ARGS__)




#ifdef SWEET_NO_UNITTEST
#define UNITTEST(...) __UTEST_NOTEST(__VA_ARGS__)
#else
#define UNITTEST(...) __UTEST(__VA_ARGS__)
#endif

#define AS_EQ(e1,e2)				UNIT_COMPARE(true,true,e1,e2,"",[](){})
#define AS_NEQ(e1,e2)				UNIT_COMPARE(true,false,e1,e2,"",[](){})
#define AS_T(e)						UNIT_COMPARE(false,true,e,true,"",[](){})
#define AS_F(e)						UNIT_COMPARE(false,true,e,false,"",[](){})
#define AS_EQ_MSG(e1,e2,msg)		UNIT_COMPARE(true,true,e1,e2,msg,[](){})
#define AS_NEQ_MSG(e1,e2,msg)		UNIT_COMPARE(true,false,e1,e2,msg,[](){})
#define AS_T_MSG(e,msg)				UNIT_COMPARE(false,true,e,true,msg,[](){})
#define AS_F_MSG(e,msg)				UNIT_COMPARE(false,true,e,false,msg,[](){})
#define AS_EQ_C(e1,e2,c)			UNIT_COMPARE(true,true,e1,e2,"",c)
#define AS_NEQ_C(e1,e2,c)			UNIT_COMPARE(true,false,e1,e2,"",c)
#define AS_T_C(e,c)					UNIT_COMPARE(false,true,e,true,"",c)
#define AS_F_C(e,c)					UNIT_COMPARE(false,true,e,false,"",c)
#define ASSERT_EQ(e1,e2)			UNIT_COMPARED(true,true,e1,e2,"",[](){})
#define ASSERT_NEQ(e1,e2)			UNIT_COMPARED(true,false,e1,e2,"",[](){})
#define ASSERT_T(e)					UNIT_COMPARED(false,true,e,true,"",[](){})
#define ASSERT_F(e)					UNIT_COMPARED(false,true,e,false,"",[](){})
#define ASSERT_EQ_C(e1,e2,e)		UNIT_COMPARED(true,true,e1,e2,"",e)
#define ASSERT_NEQ_C(e1,e2,e)		UNIT_COMPARED(true,false,e1,e2,"",e)
#define ASSERT_T_C(e,ec)			UNIT_COMPARED(false,true,e,true,"",ec)
#define ASSERT_F_C(e,ec)			UNIT_COMPARED(false,true,e,false,"",ec)
#define ASSERT_EQ_MSG(e1,e2,msg)	UNIT_COMPARED(true,true,e1,e2,msg,[](){})
#define ASSERT_NEQ_MSG(e1,e2,msg)	UNIT_COMPARED(true,false,e1,e2,msg,[](){})
#define ASSERT_T_MSG(e,msg)			UNIT_COMPARED(false,true,e,true,msg,[](){})
#define ASSERT_F_MSG(e,msg)			UNIT_COMPARED(false,true,e,false,msg,[](){})

#define IF_BREAK(e)			if(!e) return;

#define ENF_T(e) 					UNIT_COMPARED_DIE(false,true,e,true,"",[](){},false)
#define ENF_F(e) 					UNIT_COMPARED_DIE(false,false,e,false,"",[](){},false)
#define ENF_EQ(e,e2) 				UNIT_COMPARED_DIE(true,true,e,e2,"",[](){},false)
#define ENF_NEQ(e,e2) 				UNIT_COMPARED_DIE(true,false,e,e2,"",[](){},false)
#define ENF_T_MSG(e,msg) 			UNIT_COMPARED_DIE(false,true,e,true,msg,[](){},false)
#define ENF_F_MSG(e,msg) 			UNIT_COMPARED_DIE(false,false,e,false,msg,[](){},false)
#define ENF_EQ_MSG(e,e2,msg) 		UNIT_COMPARED_DIE(true,true,e,e2,msg,[](){},false)
#define ENF_NEQ_MSG(e,e2,msg) 		UNIT_COMPARED_DIE(true,false,e,e2,msg,[](){},false)
#define ENF_T_C(e,c) 				UNIT_COMPARED_DIE(false,true,e,true,"",c,false)
#define ENF_F_C(e,c) 				UNIT_COMPARED_DIE(false,false,e,false,"",c,false)
#define ENF_EQ_C(e,e2,c) 			UNIT_COMPARED_DIE(true,true,e,e2,"",c,false)
#define ENF_NEQ_C(e,e2,c) 			UNIT_COMPARED_DIE(true,false,e,e2,"",c,false)
#define ENFR_T(e) 					if(!UNIT_COMPARED_DIE(false,true,e,true,"",[](){},false)) return false;
#define ENFR_F(e) 					if(!UNIT_COMPARED_DIE(false,false,e,false,"",[](){},false)) return false;
#define ENFR_EQ(e,e2) 				if(!UNIT_COMPARED_DIE(true,true,e,e2,"",[](){},false)) return false;
#define ENFR_NEQ(e,e2) 				if(!UNIT_COMPARED_DIE(true,false,e,e2,"",[](){},false)) return false;
#define ENFR_T_MSG(e,msg) 			if(!UNIT_COMPARED_DIE(false,true,e,true,msg,[](){},false)) return false;
#define ENFR_F_MSG(e,msg) 			if(!UNIT_COMPARED_DIE(false,false,e,false,msg,[](){},false)) return false;
#define ENFR_EQ_MSG(e,e2,msg) 		if(!UNIT_COMPARED_DIE(true,true,e,e2,msg,[](){},false)) return false;
#define ENFR_NEQ_MSG(e,e2,msg) 		if(!UNIT_COMPARED_DIE(true,false,e,e2,msg,[](){},false)) return false;
#define ENFR_T_C(e,c) 				if(!UNIT_COMPARED_DIE(false,true,e,true,"",c,false)) return false;
#define ENFR_F_C(e,c) 				if(!UNIT_COMPARED_DIE(false,false,e,false,"",c,false)) return false;
#define ENFR_EQ_C(e,e2,c) 			if(!UNIT_COMPARED_DIE(true,true,e,e2,"",c,false)) return false;
#define ENFR_NEQ_C(e,e2,c) 			if(!UNIT_COMPARED_DIE(true,false,e,e2,"",c,false)) return false;

#define UNIT_COMPARE(compare,result,e1,e2,msg,c); this->increNumAsserts(); \
IF_BREAK (sweet::Unit::Unittest::evaluate(compare,result, e1, e2, #e1, #e2,\
__FILE__, __LINE__ ,msg,c))

#define UNIT_COMPARED(compare,result,e1,e2,msg,c) \
sweet::Unit::Unittest::evaluates(compare, result, e1, e2, #e1, #e2,__FILE__, __LINE__,\
&std::cout, "", true, msg, c)

#define UNIT_COMPARED_DIE(compare,result,e1,e2,msg,c,die) \
sweet::Unit::Unittest::evaluates(compare, result, e1, e2, #e1, #e2,__FILE__, __LINE__,\
&std::cout, "", die, msg, c)

namespace sweet {
namespace Unit {

	template<typename T, class Enable = void>
	struct Gen;

	template<typename T>
	struct Gen<T, typename std::enable_if<std::is_integral<T>::value>::type> {
		T l, h;
		std::uniform_int_distribution<T> distribution;

		Gen(T l = std::numeric_limits<T>::min(), 
			T h = std::numeric_limits<T>::max()) : l(l), h(h), distribution(l,h) 
		{}

		template<typename G>
		T operator()(G& gen) {
			return this->distribution(gen);
		}
	};

	template<typename T>
	struct Gen<T, typename std::enable_if<std::is_floating_point<T>::value >::type> {
		T l, h;
		std::uniform_real_distribution<T> distribution;

		Gen(T l = std::numeric_limits<T>::min(), 
			T h = std::numeric_limits<T>::max()) : l(l), h(h), distribution(l,h) 
		{}

		template<typename G>
		T operator()(G& gen) {
			return this->distribution(gen);
		}
	};

	using namespace std;
	static inline string sname(const string& str) {
		size_t idx(str.rfind('/'));
		if(idx != string::npos) {
			string ret(str);
			ret.erase(0, idx+1);	
			return ret;
		} else
			return string(str);
	}

	template<bool>
	struct comp_sel {
		template<typename T, typename S>
		static bool comp(const T t, const S s) {
			return t == s;
		}
	};

	template<>
	struct comp_sel<true> {
		template<typename T, typename S>
		static bool comp(const T t, const S s) {
			if(std::numeric_limits<T>::infinity() == t &&
					std::numeric_limits<S>::infinity() == s) {
				return true;
			}
			return fabs(t - s) <= 0.000001;
		}
	};
	
	class Unittest;

	inline vector<Unittest*>& getTests() {
		static vector<Unittest*> tests;
		return tests;
	}

	inline unsigned& getNumOfAsserts() {
		static unsigned numAsserts;
		return numAsserts;
	}

	class Unittest {
	public:
		Unittest(const string& name, std::string f, int l, int count = 1,
				std::string more = "") : file(sname(f)),
				line(l), name_(name),  info(more), numRounds(count), 
				errors_(0), out_(&cerr) 
		{
			getTests().push_back(this);
		}

		inline void increNumAsserts() {
			getNumOfAsserts()++;
		}

#ifdef SWEET_NO_ASSERTS
		template<typename E1, typename E2, typename C> 
		static bool evaluates(bool , bool , const E1& , 
				const E2& , const char* , const char* , 
				const char* , int , ostream* , const string& , 
				bool , const string& , C ) {
			return true;
		}
#else
		template<typename E1, typename E2, typename C> 
		static bool evaluates(bool compare, bool result, const E1& e1, 
				const E2& e2, const char* str1, const char* str2, 
				const char* file, int line, ostream* out, const string& name, 
				bool die, const string& msg, C c) 
		{

			if(result ? 
					(comp_sel<is_floating_point<E1>::value>::comp(e1, e2)) :
					(!comp_sel<is_floating_point<E1>::value>::comp(e1, e2))) 
			{
				return true;
			}

			if(name != "") {
				*out<<sname(file)<< ":"<<line<<" in "<<"Unittest("<<name<<
					") Assert Failed: ";
			} else {
				*out<<sname(file)<< ":"<<line<<" Assert Failed: ";
			}
			stringstream s2;
			s2<<std::fixed<<std::setprecision(9)<<boolalpha<<(e2);

			if(compare) {
				stringstream s1;
				s1<<std::fixed<<std::setprecision(9)<<boolalpha<<(e1);
  
				const string cmp(result ? "==" : "!=");
				*out<<"compare {"<<str1<<"} "<< cmp<<" {"<<str2<<"} "
					<<"got {\""<<s1.str()<<"\"} "<<cmp<<" {\""<<s2.str()
					<< "\"}";
			} else {
				*out<<"evalute {"<<str1<<"} == "<<s2.str();
			}
			*out<<' '<<msg<<endl;

			c();

			if(die) {
				exit(1);
			}
			return false;
		}
#endif

		template<typename E1, typename E2, typename C> 
		bool evaluate(bool compare, bool result, const E1& e1, const E2& e2, 
			const char* str1, const char* str2, const char* file, int line, 
			const string& msg, C c) 
		{
			bool rlst = Unittest::evaluates<E1,E2,C>(compare, result, e1, e2,
					str1, str2, file, line, out_, name_, false, msg, c);
			if(!rlst) {
				++errors_;
			}

			return rlst;
		}

#ifdef SWEET_NO_UNITTEST
		void run_impl() {}
		virtual void nevercall() = 0;
#else
		virtual void run_impl() = 0;
#endif

		bool run() {
			run_impl();
			return errors_;
		}

		void setOutputStream(ostream* o) {
			out_ = o;
		}

		std::string file;
		int line;
		std::string name_;
		std::string info;
		int numRounds;

	private:
		int errors_;
		ostream* out_;
	};

	inline unsigned runTests(std::string benmarkrslt = 
			"UnittestBenchmarkResult.ben") 
	{
		char timeStr[100];
		std::time_t now_time = std::time(NULL);
		std::strftime(timeStr, 100, "%Y:%m:%d-%H:%M:%S",
				std::localtime(&now_time));
		unsigned rs = 0;
		for(vector<Unittest*>::iterator it = getTests().begin(); it !=
				getTests().end(); ++it) {
			std::chrono::time_point<std::chrono::system_clock> strt(
				std::chrono::system_clock::now());
			try {
				for(int i = 0; i < (*it)->numRounds; ++i) {
					bool tmp = (*it)->run();
					rs += tmp;
				}
			} catch(std::exception& e) {
				std::cerr<<(*it)->file<<":"<<(*it)->line<<" Unittest"<<
					(*it)->name_<<" has thrown an "<< "uncaught exception "<<
					" with message "<<e.what()<<std::endl;
				++rs;
			} catch(...) {
				std::cerr<<(*it)->file<<":"<<(*it)->line<<" Unittest"<<
					(*it)->name_<<" has thrown an "<< "uncaught exception "
					<<std::endl;
				++rs;
			}
			std::chrono::time_point<std::chrono::system_clock> stp(
				std::chrono::system_clock::now());

			if((*it)->numRounds > 1) {
				std::ofstream o(benmarkrslt, std::ios::app);	
				o<<(*it)->name_<<':'<<(*it)->line<<':'<<(*it)->file<<' '<<
					timeStr<<' '<<((*it)->info == "" ? "" : (*it)->info + " ")<<
				   	std::chrono::duration_cast<std::chrono::milliseconds>(
						stp-strt
					).count()<<std::endl;
			}
		}
		return rs;
	}
}
}
