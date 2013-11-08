#pragma once

#include <unordered_set>
#include <vector>

#include <rulestore.hpp>
#include <output.hpp>

struct RulePartCC {
	bool operator()(const RulePart& a, const RulePart& b) {
		if(a.name == b.name) {
			if(a.storeName == b.storeName) {
				return a.endName < b.endName;
			}
			return a.storeName < b.storeName;
		}
		return a.name < b.name;
	}
};

struct ErrorStuff {
	const std::string rule;
	const std::string part;
	const size_t depth;
	const std::vector<std::string> possible;
	ErrorStuff(const std::string&, const std::string&, const size_t, const std::vector<std::string>&);
};

class RecurDec {
	typedef Trie<RulePart,std::string> GrammarPrefix;
	typedef Trie<RulePart,std::string,RulePartCC> GrammarPrefixEnum;
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

	std::vector<std::vector<RulePart>> store;
	std::vector<std::vector<RulePart>> store2;

	//void walkTrieConstructor(std::vector<std::string>);
	void walkTrieConstructor(const GrammarPrefix::TrieEntry*,
		std::vector<RulePart>);

	void walkTrieConstructor2(const GrammarPrefixEnum::TrieEntry*,
		std::vector<RulePart>);

	void genAst(const std::vector<std::vector<RulePart>>&);
	void genAstClassDeclStart();
	void genAstClassDeclEnd(const std::set<RulePart>&);
	void genAstOutputMethods(const std::map<std::string,std::vector<RulePart>>&);

public:
	RuleStore& rs;

	RecurDec(RuleStore&,Output&);

	void computeFirstSet();
	void gen();
};
