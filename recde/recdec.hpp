#pragma once

#include <unordered_set>
#include <rulestore.hpp>

class RecurDec {
	std::unordered_set<std::string> allreadyDone;
	void genRules(const std::string&);
	std::ostream& headerS;
	std::ostream& srcS;

public:
	RuleStore& rs;

	RecurDec(RuleStore&,std::ostream&,std::ostream&);

	void computeFirstSet();
	void genRules();
};
