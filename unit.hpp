/* unit.hpp - a simple unit test framework for C++ based on QUnit and Testdog
Author: Robert "burner" Schadek rburners@gmail.com License: LGPL

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

int main() {
	if(!Unit::runTests()) {
		return 1;
	}
}
*/

#ifndef SWEET_UNIT
#define SWEET_UNIT
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>

#define UNITTEST(test_name) \
class test_name##_test_class : public Unit::Unittest { void run_impl(); \
public: \
test_name##_test_class() : Unit::Unittest(#test_name,__FILE__,__LINE__) {} \
} test_name##_test_class_impl; \
void test_name##_test_class::run_impl()

#define AS_EQ(e1,e2)	UNIT_COMPARE(true,true,e1,e2)
#define AS_NEQ(e1,e2)	UNIT_COMPARE(true,false,e1,e2)
#define AS_T(e)			UNIT_COMPARE(false,true,e,true)
#define AS_F(e)			UNIT_COMPARE(false,true,e,false)

#define UNIT_COMPARE(compare,result,e1,e2) evaluate(compare,			  \
result, e1, e2, #e1, #e2,Unit::sname(__FILE__), __LINE__)

namespace Unit {
	using namespace std;
	static string sname(const string& str) {
		size_t idx(str.rfind('/'));
		if(idx != string::npos) {
			string ret(str);
			ret.erase(0, idx+1);	
			return ret;
		} else
			return string(str);
	}

	template<typename T> 
	class is_float {
		public:
		static const bool value = false;
	};

	template<> 
	class is_float<float> {
		public:
		static const bool value = true;
	};

	template<> 
	class is_float<double> {
		public:
		static const bool value = true;
	};

	template<> 
	class is_float<long double> {
		public:
		static const bool value = true;
	};

	template<typename T, typename S, typename R>
	struct comp {
		bool operator()(T t, S s) {
			return t == s;
		}
	};

	template<typename T, typename S>
	//template<typename T, typename S, typename R>
	template<>
	struct comp<T,S,true> {
		bool operator()(T t, S s) {
			return fabs(t - s) < 0.0001;
		}
	};
	
	class Unittest;

	inline vector<Unittest*>& getTests() {
		static vector<Unittest*> tests;
		return tests;
	}

	class Unittest {
	public:
		Unittest(const string& name, std::string f, int l) : file(sname(f)),
				line(l), name_(name), errors_(0), out_(&cerr) {
			getTests().push_back(this);
		}

		template<typename E1, typename E2> bool evaluate(bool compare, 
				bool result, const E1& e1, const E2& e2, const string& str1, 
				const string& str2, const string& file, int line) {
			stringstream s1, s2;
			s1<<boolalpha<<(e1);
			s2<<boolalpha<<(e2);

			//if(result ? (e1 == e2) : (e1 != e2)) return true;
			if(result ? (comp<const E1&, const E2&, is_float<E1>::value>()(e1,e2)) : 
				(!comp<const E1&, const E2&, is_float<E1>::value>()(e1, e2))) return true;
			++errors_;
  
			*out_<<sname(file)<< ":"<<line<<" in "<<"Unittest("<<name_<<
				") Assert Failed: ";
			if(compare) {
				const string cmp(result ? "==" : "!=");
				*out_<<"compare {"<<str1<<"} "<< cmp<<" {"<<str2<<"} "<<"got {\""<<
					s1.str()<<"\"} "<<cmp<<" {\""<<s2.str()<< "\"}";
			} else
				*out_<<"evalute {"<<str1<<"} == "<<s2.str();
			*out_<<endl;
			return false;
		}

		virtual void run_impl() = 0;

		bool run() {
			run_impl();
			return errors_;
		}

		void setOutputStream(ostream* o) {
			out_ = o;
		}

		std::string file;
		int line;
		string name_;

	private:
		int errors_;
		ostream* out_;
	};

	inline bool runTests() {
		bool rs(true);
		for(vector<Unittest*>::iterator it = getTests().begin(); it !=
				getTests().end(); ++it) {
			try {
				bool tmp = (*it)->run();
				rs &= !tmp;
			} catch(std::exception& e) {
				std::cerr<<(*it)->file<<":"<<(*it)->line<<" Unittest"<<
					(*it)->name_<<" has thrown an "<< "uncaught exception "<<
					" with message "<<e.what()<<std::endl;
				rs &= false;
			} catch(...) {
				std::cerr<<(*it)->file<<":"<<(*it)->line<<" Unittest"<<
					(*it)->name_<<" has thrown an "<< "uncaught exception "
					<<std::endl;
				rs &= false;
			}
		}
		return rs;
	}
}
#endif
