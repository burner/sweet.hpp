#pragma once

#include <unordered_set>
#include <vector>

#include <rulestore.hpp>
#include <output.hpp>

struct ErrorStuff {
	const std::string rule;
	const std::string part;
	const size_t depth;
	const std::vector<std::string> possible;
	ErrorStuff(const std::string&, const std::string&, const size_t, const std::vector<std::string>&);
};

class RecurDec {
	typedef Trie<RulePart,bool> GrammarPrefix;
	std::unordered_set<std::string> allreadyDone;
	std::vector<std::string> nameStack;
	std::string current;
	std::vector<ErrorStuff> errorStuff;

	size_t newErrorStuff(const std::string&,const std::string&, const size_t,
		const std::vector<std::string>&);

	void genRules(const std::string&);
	void writeErrorStuff();
	void walkTrie(const GrammarPrefix::TrieEntry*, const std::string&, const size_t);
	void genRules();
	void genAstForwardDecl();

	Output& out;

	std::vector<std::vector<std::string>> store;
	void walkTrieConstructor(std::vector<std::string>);
	void walkTrieConstructor(const GrammarPrefix::TrieEntry*,
		std::vector<std::string>);

public:
	RuleStore& rs;

	RecurDec(RuleStore&,Output&);

	void computeFirstSet();
	void gen();
};
