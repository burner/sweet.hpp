#pragma once 

#include <vector>
#include <ostream>
#include <rulepart.hpp>

typedef std::vector<RulePart> RuleVector;

struct Expr {
    friend std::ostream& operator<<(std::ostream&, const Expr&);
	RuleVector rule;
	Expr(const RuleVector&);
};
