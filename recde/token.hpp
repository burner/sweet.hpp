#pragma once

#include <string>

struct Token {
	std::string name;
	std::string regex;
	std::string convertFunction;
	bool storeAnything;

	Token(const std::string&, const std::string&, const std::string&, bool);
};
