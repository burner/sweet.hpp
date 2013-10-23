#pragma once

#include <string>
#include <ostream>

struct RulePart {
    friend std::ostream& operator<<(std::ostream&, const RulePart&);

	std::string name;
	std::string storeName;

	RulePart(const std::string&, const std::string&);
	bool operator<(const RulePart&) const;
};
