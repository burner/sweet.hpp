/* unit.hpp - a small but elegant unit test framework for C++ based on QUnit and Testdog
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
	if(!Unit::runTests("TheOptionalNameOfTheFileWithTheBenchmarkResults")) {
		return 1;
	}
}
*/

#ifndef SWEET_UNIT_HPP
#define SWEET_UNIT_HPP
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath>
#include <type_traits>
#include <limits>

#define PP_HAS_ARGS_IMPL2(_1, _2, _3, N, ...) N
#define PP_HAS_ARGS_SOURCE() MULTI, MULTI, ONE, ERROR
#define PP_HAS_ARGS_IMPL(...) PP_HAS_ARGS_IMPL2(__VA_ARGS__)
#define PP_HAS_ARGS(...)      PP_HAS_ARGS_IMPL(__VA_ARGS__, PP_HAS_ARGS_SOURCE())


#define __UTEST_NOTEST_ONE(test_name) \
class test_name##_test_class : public Unit::Unittest { void nevercall(); \
public: \
test_name##_test_class() : Unit::Unittest(#test_name,__FILE__,__LINE__) {} \
} test_name##_test_class_impl; \
void test_name##_test_class::nevercall()

#define __UTEST_NOTEST_MULTI(test_name,...) \
class test_name##_test_class : public Unit::Unittest { void nevercall(); \
public: \
test_name##_test_class() : Unit::Unittest(#test_name,__FILE__,__LINE__,__VA_ARGS__) {} \
} test_name##_test_class_impl; \
void test_name##_test_class::nevercall()

#define __UTEST_NOTEST_DISAMBIGUATE2(has_args, ...) __UTEST_NOTEST_ ## has_args (__VA_ARGS__)
#define __UTEST_NOTEST_DISAMBIGUATE(has_args, ...) __UTEST_NOTEST_DISAMBIGUATE2(has_args, __VA_ARGS__)
#define __UTEST_NOTEST(...) __UTEST_NOTEST_DISAMBIGUATE(PP_HAS_ARGS(__VA_ARGS__), __VA_ARGS__)


#define __UTEST_ONE(test_name) \
class test_name##_test_class : public Unit::Unittest { void run_impl(); \
public: \
test_name##_test_class() : Unit::Unittest(#test_name,__FILE__,__LINE__) {} \
} test_name##_test_class_impl; \
void test_name##_test_class::run_impl()

#define __UTEST_MULTI(test_name,...) \
class test_name##_test_class : public Unit::Unittest { void run_impl(); \
public: \
test_name##_test_class() : Unit::Unittest(#test_name,__FILE__,__LINE__,__VA_ARGS__) {} \
} test_name##_test_class_impl; \
void test_name##_test_class::run_impl()

#define __UTEST_DISAMBIGUATE2(has_args, ...) __UTEST_ ## has_args (__VA_ARGS__)
#define __UTEST_DISAMBIGUATE(has_args, ...) __UTEST_DISAMBIGUATE2(has_args, __VA_ARGS__)
#define __UTEST(...) __UTEST_DISAMBIGUATE(PP_HAS_ARGS(__VA_ARGS__), __VA_ARGS__)


#ifdef SWEET_NO_UNITTEST
#define UNITTEST(...) __UTEST_NOTEST(__VA_ARGS__)
#else
#define UNITTEST(...) __UTEST(__VA_ARGS__)
#endif

#define AS_EQ(e1,e2)				UNIT_COMPARE(true,true,e1,e2,"")
#define AS_NEQ(e1,e2)				UNIT_COMPARE(true,false,e1,e2,"")
#define AS_T(e)						UNIT_COMPARE(false,true,e,true,"")
#define AS_F(e)						UNIT_COMPARE(false,true,e,false,"")
#define AS_EQ_MSG(e1,e2,msg)		UNIT_COMPARE(true,true,e1,e2,msg)
#define AS_NEQ_MSG(e1,e2,msg)		UNIT_COMPARE(true,false,e1,e2,msg)
#define AS_T_MSG(e,msg)				UNIT_COMPARE(false,true,e,true,msg)
#define AS_F_MSG(e,msg)				UNIT_COMPARE(false,true,e,false,msg)
#define ASSERT_EQ(e1,e2)			UNIT_COMPARED(true,true,e1,e2,"")
#define ASSERT_NEQ(e1,e2)			UNIT_COMPARED(true,false,e1,e2,"")
#define ASSERT_T(e)					UNIT_COMPARED(false,true,e,true,"")
#define ASSERT_F(e)					UNIT_COMPARED(false,true,e,false,"")
#define ASSERT_EQ_MSG(e1,e2,msg)	UNIT_COMPARED(true,true,e1,e2,msg)
#define ASSERT_NEQ_MSG(e1,e2,msg)	UNIT_COMPARED(true,false,e1,e2,msg)
#define ASSERT_T_MSG(e,msg)			UNIT_COMPARED(false,true,e,true,msg)
#define ASSERT_F_MSG(e,msg)			UNIT_COMPARED(false,true,e,false,msg)

#define IF_BREAK(e)			if(!e) return;

#define UNIT_COMPARE(compare,result,e1,e2,msg) IF_BREAK(evaluate(compare,result, \
e1, e2, #e1, #e2,Unit::sname(__FILE__), __LINE__,msg))
#define UNIT_COMPARED(compare,result,e1,e2,msg) Unit::Unittest::evaluates(compare,  \
result, e1, e2, #e1, #e2,Unit::sname(__FILE__), __LINE__, &std::cout, "", true, msg)

namespace Unit {
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

	class Unittest {
	public:
		Unittest(const string& name, std::string f, int l, int count = 1,
				std::string more = "") : file(sname(f)),
				line(l), name_(name),  info(more), numRounds(count), 
				errors_(0), out_(&cerr) {
			getTests().push_back(this);
		}

		template<typename E1, typename E2> static bool evaluates(bool compare, 
				bool result, const E1& e1, const E2& e2, const string& str1, 
				const string& str2, const string& file, int line,
				ostream* out, const string& name, bool die, const string& msg) {

#ifndef SWEET_NO_ASSERTS
			//if(result ? (e1 == e2) : (e1 != e2)) return true;
			if(result ? 
					(comp_sel<is_floating_point<E1>::value>::comp(e1, e2)) :
					(!comp_sel<is_floating_point<E1>::value>::comp(e1, e2))) {
				return true;
			}

			if(name != "") {
				*out<<sname(file)<< ":"<<line<<" in "<<"Unittest("<<name<<
					") Assert Failed: ";
			} else {
				*out<<sname(file)<< ":"<<line<<" Assert Failed: ";
			}
			stringstream s2;
			s2<<boolalpha<<(e2);

			if(compare) {
				stringstream s1;
				s1<<boolalpha<<(e1);
  
				const string cmp(result ? "==" : "!=");
				*out<<"compare {"<<str1<<"} "<< cmp<<" {"<<str2<<"} "<<"got {\""<<
					s1.str()<<"\"} "<<cmp<<" {\""<<s2.str()<< "\"}";
			} else {
				*out<<"evalute {"<<str1<<"} == "<<s2.str();
			}
			*out<<' '<<msg<<endl;
			if(die) {
				exit(1);
			}
			return false;
#else
			return true;
#endif
		}

		template<typename E1, typename E2> bool evaluate(bool compare, 
				bool result, const E1& e1, const E2& e2, const string& str1, 
				const string& str2, const string& file, int line, const string& msg) {
			bool rlst = Unittest::evaluates<E1,E2>(compare, result, e1, e2,
					str1, str2, file, line, out_, name_, false, msg);
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

	inline unsigned runTests(std::string benmarkrslt = "UnittestBenchmarkResult.ben") {
		char timeStr[100];
		std::time_t now_time = std::time(NULL);
		std::strftime(timeStr, 100, "%Y:%m:%d-%H:%M:%S",
				std::localtime(&now_time));
		//bool rs(true);
		unsigned rs = 0;
		for(vector<Unittest*>::iterator it = getTests().begin(); it !=
				getTests().end(); ++it) {
			std::chrono::time_point<std::chrono::system_clock> strt(
				std::chrono::system_clock::now());
			try {
				for(int i = 0; i < (*it)->numRounds; ++i) {
					bool tmp = (*it)->run();
					//rs &= !tmp;
					rs += tmp;
				}
			} catch(std::exception& e) {
				std::cerr<<(*it)->file<<":"<<(*it)->line<<" Unittest"<<
					(*it)->name_<<" has thrown an "<< "uncaught exception "<<
					" with message "<<e.what()<<std::endl;
				//rs &= false;
				++rs;
			} catch(...) {
				std::cerr<<(*it)->file<<":"<<(*it)->line<<" Unittest"<<
					(*it)->name_<<" has thrown an "<< "uncaught exception "
					<<std::endl;
				//rs &= false;
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
#endif
