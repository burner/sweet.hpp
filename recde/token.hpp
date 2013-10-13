#pragma once

#include <ostream>
#include <string>
#include <format.hpp>

struct Token {
    friend std::ostream& operator<<(std::ostream&, const Token&);

	std::string name;
	std::string regex;
	std::string convertFunction;

	Token(const std::string&, const std::string&, const std::string&);
};
