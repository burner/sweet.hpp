// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
#pragma once

#include <string>

namespace sweet {

struct Option {
	std::string l;
	std::string s;
};

typedef std::vector<Option> Options;

typedef std::pair<bool, Options> GetOptResult;

template<typename... Args>
GetOptResult getopts(const std::string& desc, int argc, char** argv, Args... args) {
	if(argc == 0) {
		return GetOptResult(false, Options());
	}

	bool help;
	Options opt;
	opt.reserve(argc/2);

	return GetOptResult(help, opt);
}

}
