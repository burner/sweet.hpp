// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
#ifndef SWEET_LOGGER
#define SWEET_LOGGER

#include <iostream>
#include "format.hpp"

#ifndef RELEASE
static std::string shortenString(const std::string& str) {
	size_t idx = str.rfind('/');
	if(idx == std::string::npos) {
		return str;
	} else {
		return str.substr(idx+1);
	}
}

struct Log {
	private:
		std::string fn;
		int line;
		bool warn;
	public:
	Log(const char* f, int l, bool w = false) {
		fn = shortenString(f);
		line = l;
		warn = w;
	}

	void operator()() {
		format(std::cerr, "%s:%d ", fn, line);
		std::cerr<<std::endl;
	}

	template<typename... Args>
	void operator()(std::string form, Args... args) {
		format(std::cerr, "%s:%d ", fn, line);
		format(std::cerr, form, args...);
		std::cerr<<std::endl;
	}

	/*template<typename... Args>
	void operator()(bool p, std::string form, Args... args) {
		if(p) {
			if(warn) {
				std::cerr<<"WARN ";
			}
			format(std::cerr, "%s:%d ", fn, line);
			format(std::cerr, form, args...);
			std::cerr<<std::endl;
		}
	}*/
};
#define LOG Log(__FILE__,__LINE__)
#define WARN Log(__FILE__,__LINE__,true)
#else
struct Log {
	Log(const char*, int l, bool w = false) { }
	void operator()() {
	}
	template<typename... Args>
	void operator()(std::string form, Args... args) {
	}
	/*template<typename... Args>
	void operator()(bool b, std::string form, Args... args) {
	}*/
};
#define LOG Log(__FILE__, __LINE__)
#define WARN Log(__FILE__, __LINE__,true) 
#endif
#endif
