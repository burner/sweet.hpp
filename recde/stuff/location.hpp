#pragma once

#include <string>

struct Loc {
	public:
	std::string file;
	unsigned line;
	unsigned column;

	Loc();
	Loc(unsigned, unsigned);
	Loc(std::string, unsigned, unsigned);
	
	std::string toString() const;
};
