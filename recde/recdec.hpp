#pragma once

#include <unordered_set>
#include <stack>

#include <rulestore.hpp>
#include <output.hpp>

class RecurDec {
	typedef Trie<RulePart,bool> GrammarPrefix;
	std::unordered_set<std::string> allreadyDone;
	std::vector<std::string> nameStack;
	std::string current;

	void genRules(const std::string&);
	Output& out;

	void walkTrie(const GrammarPrefix::TrieEntry*, const std::string&, const size_t);

public:
	RuleStore& rs;

	RecurDec(RuleStore&,Output&);

	void computeFirstSet();
	void genRules();
};
