#pragma once

#include <rulestore.hpp>
#include <sjson.hpp>

/*
{
	{"Token" : [
		{ "Name" : "Semicolon", "Regex" : ";", "ConvertFunction" : "nothing" },
		{ "Name" : "Integer", "Regex" : "[0-9]+", "ConvertFunction" : "toInteger" }
	]},
	{"Rules" : [
		{ "A" : [
			{ "Rule" : "Whitespace separated" },
			{ "Rule" : "Another Whitespace separated" }
			]
		},
		{ "B" : [
			{ "Rule" : "Whitespace separated" },
			{ "Rule" : "Another Whitespace separated" }
			]
		}
	]}
}

*/


struct RuleParser {
	std::string str;

	TokenMap& token;
	RuleMultiMap& ruleMap;

	RuleParser(const std::string&,TokenMap&, RuleMultiMap&);
	void parse();
};
