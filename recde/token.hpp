#pragma once

struct Token {
	std::string name;
	std::string regex;
	std::string typeToCovertTo;
	bool storeAnything;
};
