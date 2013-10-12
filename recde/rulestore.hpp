#pragma once

#include <unordered_map>
#include <string>
#include <expr.hpp>
#include <token.hpp>

typedef std::unordered_multimap<std::string,Expr> RuleMultiMap;
typedef std::unordered_map<std::string,Token> TokenMap;

struct RuleStore {
	RuleMultiMap rules;
	TokenMap token;
};
