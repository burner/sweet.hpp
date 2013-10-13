#include <rulepart.hpp>
#include <format.hpp>

RulePart::RulePart(const std::string& n, const std::string& s) : name(n), storeName(s) {
}

std::ostream& operator<<(std::ostream& s, const RulePart& t) {
	format(s, "RulePart(name('%s'), storeName('%s'))", t.name, t.storeName);
	return s;
}
