#include <algorithm>
#include <ruleparser.hpp>
#include <unit.hpp>
#include <logger.hpp>

// trim from both ends
static inline std::string trim(const std::string& s, const std::string tt = "\t \n") {
	std::string ret = s;
	size_t endpos = ret.find_last_not_of(tt);
	if(std::string::npos != endpos) {
	    ret = ret.substr( 0, endpos+1 );
	}
	size_t startpos = ret.find_first_not_of(" \t");
	if(std::string::npos != startpos) {
	    ret = ret.substr( startpos );
	}
	return ret;
}

static std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


static std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

RuleParser::RuleParser(const std::string& s, RuleStore& rs) : str(s), token(rs.token), 
	ruleMap(rs.rules) {
}

void RuleParser::parse() {
	sweet::jsonparser jp(str);

	std::string tokStr("Token");
	if(jp.getRoot()->pathExists(tokStr)) {
		auto tok = jp.getRoot()->access(tokStr);
		auto tokType = tok->getType();
		ASSERT_EQ(tokType, sweet::value::type_array);

		for(auto& it : tok->getArray()) {
			std::string name = it->getObject()->access("Name")->getString();
			std::string regex = it->getObject()->access("Regex")->getString();
			std::string convertFunc;
			if(it->getObject()->pathExists("ConvertFunction")) {
				convertFunc = it->getObject()->access("ConvertFunction")->getString();
			}
			token.insert(std::make_pair(name,
				Token(name, regex, convertFunc)
			));
		}
	}

	std::string ruleStr("Rules");
	if(jp.getRoot()->pathExists(ruleStr)) {
		auto rls = jp.getRoot()->access(ruleStr);
		auto rlsType = rls->getType();
		ASSERT_EQ(rlsType, sweet::value::type_array);
		for(auto& it : rls->getArray()) {

			std::string ruleName = it->getObject()->access("Name")->getString();
			auto& entry = it->getObject()->access("Expression")->getArray();
			for(auto& jt : entry) {
				RuleVector rv;
				auto semiSplit = split(jt->getObject()->access("Rule")->getString(), ';');
				std::string ruleEnd = jt->getObject()->get<std::string>("Id", "");
				for(auto& ht : semiSplit) {
					std::string type = trim(ht.substr(0, ht.find('(')));
					size_t lParen = ht.find('(');
					size_t rParen = ht.find(')');
					std::string saveName;
					if(lParen != std::string::npos && rParen != std::string::npos) {
						saveName = trim(ht.substr(lParen+1, rParen), "\t ()");
					}

					rv.push_back(RulePart(type, saveName, ruleEnd));
				}

				ruleMap.insert(std::make_pair(ruleName, Expr(rv)));
			}

		}
	}
}
