// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
#pragma once

#include <iostream>
#include <unordered_set>

#include "format.hpp"
namespace SweetLogger {

#ifndef SWEET_NO_LOGGER
inline std::unordered_set<size_t>& getAvaiLogger() {
	static std::unordered_set<size_t> logger;
	return logger;
}

inline bool enableLogger(const size_t id) {
	return getAvaiLogger().insert(id).second;
}

inline bool disableLogger(const size_t id) {
	return static_cast<bool>(getAvaiLogger().erase(id));
}

inline std::string shortenString(const std::string& str) {
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
	explicit Log(const char* f, int l, bool w = false) {
		fn = shortenString(f);
		line = l;
		warn = w;
	}

	void operator()() {
		format(std::cerr, "%s:%d ", fn, line);
		std::cerr<<std::endl;
	}

	void operator()(const size_t ll) {
		if(getAvaiLogger().count(ll)) {
			format(std::cerr, "%s:%d ", fn, line);
			std::cerr<<std::endl;
		}
	}

	template<typename... Args>
	void operator()(const std::string& form, Args... args) {
		if(warn) {
			std::cerr<<"WARN ";
		}
		format(std::cerr, "%s:%d ", fn, line);
		format(std::cerr, form, args...);
		std::cerr<<std::endl;
	}

	template<typename... Args>
	void operator()(const size_t ll, const std::string& form, Args... args) {
		if(getAvaiLogger().count(ll)) {
			if(warn) {
				std::cerr<<"WARN ";
			}
			format(std::cerr, "%s:%d ", fn, line);
			format(std::cerr, form, args...);
			std::cerr<<std::endl;
		}
	}
};
#define LOG SweetLogger::Log(__FILE__,__LINE__)
#define WARN SweetLogger::Log(__FILE__,__LINE__,true)

#else
struct Log {
	explicit Log(const char*, int, bool = false) { }
	void operator()() {
	}
	template<typename... Args>
	void operator()(std::string, Args...) {
	}
	template<typename... Args>
	void operator()(size_t b, std::string, Args... ) {
	}
};
inline bool enableLogger(const size_t) {
	return false;
}
inline bool disableLogger(const size_t) {
	return false;
}
#define LOG SweetLogger::Log(__FILE__, __LINE__)
#define WARN SweetLogger::Log(__FILE__, __LINE__,true) 
#endif
}
