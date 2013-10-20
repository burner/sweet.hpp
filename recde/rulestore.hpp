#pragma once

#include <unordered_map>
#include <set>
#include <ostream>
#include <string>
#include <expr.hpp>
#include <token.hpp>
#include <trie.hpp>

typedef std::unordered_multimap<std::string,Expr> RuleMultiMap;
typedef std::unordered_map<std::string,std::set<std::string>> FirstSets;
typedef std::unordered_map<std::string,Token> TokenMap;

std::ostream& operator<<(std::ostream&, const FirstSets&);

struct RuleStore {
	RuleMultiMap rules;
	TokenMap token;
	FirstSets first;
};
