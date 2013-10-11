#include <ruleparser.hpp>
#include <unit.hpp>

RuleParser::RuleParser(const std::string& s, TokenMap& t, RuleMultiMap& r) : str(s), token(t), ruleMap(r) {
}

char RuleParser::peek() {
	//return *expressionToParse;
	if(pos >= str.size()) {
		return '\0';
	}
	return str[pos];
}

bool RuleParser::isWhiteSpace(const char c) {
	return c == ' ' || c == '\t' || c == '\n';
}

void eatWhiteSpace() {
	if(!isWhiteSpace(peek())) {
		return;
	}
	while(isWhiteSpace(peek())) {
	}
}

char RuleParser::get() {
	//return *expressionToParse++;
	if(pos >= str.size()) {
		throw std::string("Out of bound");
	}
	char ret = str[pos++];
	while(isWhiteSpace(str[pos])) {
		pos++;
	}
	return ret;
}

void RuleParser::parse() {

}

void RuleParser::parseComment() {
	ASSERT_EQ(peek(), '#');
	while(get() != '\n') {}
}

void RuleParser::parseRule() {
}

void RuleParser::parseRules() {

}

void RuleParser::parseToken() {
	while(true) {
		if(peek() == '!' || peek() == '\0') {
			return
		}

		std::string tokenName;
		std::string regex;
		std::string covertFunction;
		bool keep;
	}
}
