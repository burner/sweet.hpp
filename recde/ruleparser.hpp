#pragma once

#include <rulestore.hpp>
#include <sjson.hpp>

/*
{
	"Token" : [
		{ "Name" : "Semicolon", "Regex" : ";" },
	],
	"Rules" : [
		{ "A" : [
			{ "Rule" : "Whitespace separated" },
			{ "Rule" : "Another Whitespace separated" }
			]
		},

*/


struct RuleParser {
	std::string str;
	size_t pos;

	TokenMap& token;
	RuleMultiMap& ruleMap;

	RuleParser(const std::string&,TokenMap&, RuleMultiMap&);
};
