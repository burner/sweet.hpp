#include <rulepart.hpp>
#include <format.hpp>

RulePart::RulePart(const std::string& n, const std::string& s) : name(n), storeName(s) {
}

RulePart::RulePart(const std::string& n, const std::string& s, const std::string& e) 
	: name(n), storeName(s), endName(e) 
{
}

bool RulePart::operator<(const RulePart& o) const {
	return this->name < o.name;
	/*if(this->name == o.name) {
		if(this->storeName == o.storeName) {
			return this->endName < o.endName;
		}
		return this->storeName < o.storeName;
	}
	return this->name < o.name;*/
}

std::ostream& operator<<(std::ostream& s, const RulePart& t) {
	format(s, "RulePart(name('%s'), storeName('%s'), endName('%s')", t.name, 
		t.storeName, t.endName
	);
	return s;
}
