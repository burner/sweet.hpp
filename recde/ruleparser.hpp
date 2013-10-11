#pragma once

#include <rulestore.hpp>

/*

Comments start with # and fill the line

TokenName ',' regex ',' ('true','false') ',' convertFunction ';'

!

ExprName § (Entry+ ;)+ §

Entry := string '(' ('true'|'false') ',' [alpha]+ ')' ' '

*/


struct RuleParser {
	std::string str;
	size_t pos;

	TokenMap& token;
	RuleMultiMap& ruleMap;

	RuleParser(const std::string&,TokenMap&, RuleMultiMap&);

	bool isWhiteSpace(const char);
	void eatWhiteSpace();
	char peek();
	char get();
	void parse();
	void parseComment();
	void parseRule();
	void parseRules();
	void parseToken();
};
