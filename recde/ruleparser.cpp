#include <algorithm>
#include <ruleparser.hpp>
#include <unit.hpp>
#include <logger.hpp>

RuleParser::RuleParser(const std::string& s, TokenMap& t, RuleMultiMap& r) : str(s), token(t), ruleMap(r) {
}
