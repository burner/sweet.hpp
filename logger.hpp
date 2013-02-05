#ifndef SWEET_LOGGER
#define SWEET_LOGGER

#include <iostream>
#include "format.hpp"

static std::string shortenString(const std::string& str) {
	size_t idx = str.rfind('/');
	if(idx == std::string::npos) {
		return str;
	} else {
		return str.substr(idx+1);
	}
}

struct Log {
	public:
	Log(const char* f, int l, bool w = false) {
		if(w) {
			std::cerr<<"WARN ";
		}
		std::cerr<<shortenString(f)<<':'<<l<<' ';
	}

	void operator()() {
		std::cerr<<std::endl;
	}

	template<typename... Args>
	void operator()(std::string form, Args... args) {
		format(std::cerr, form, args...);
		std::cerr<<std::endl;
	}

	template<typename... Args>
	void operator()(bool p, std::string form, Args... args) {
		if(p) {
			format(std::cerr, form, args...);
			std::cerr<<std::endl;
		}
	}
};

#ifndef RELEASE
#define LOG Log(__FILE__,__LINE__)
#define WARN Log(__FILE__,__LINE__,true)
#else
#define LOG() ;
#define WARN() ;
#endif
#endif
