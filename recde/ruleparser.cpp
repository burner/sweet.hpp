#include <algorithm>
#include <ruleparser.hpp>
#include <unit.hpp>
#include <logger.hpp>

RuleParser::RuleParser(const std::string& s, TokenMap& t, RuleMultiMap& r) : str(s), token(t), ruleMap(r) {
}

void RuleParser::parse() {
	sjson::jsonparser jp(str);

	std::string tokStr("Token");
	if(jp.getRoot()->pathExists(tokStr)) {
		auto tok = jp.getRoot()->access(tokStr);
		auto tokType = tok->getType();
		ASSERT_EQ(tokType, sjson::value::type_array);

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
}

