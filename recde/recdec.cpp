#include <recdec.hpp>
#include <unit.hpp>

RecurDec::RecurDec(RuleStore& r, std::ostream& h, std::ostream& s) :  headerS(h), srcS(s), rs(r) {
}

void RecurDec::computeFirstSet() {
	bool changed = false;
	do {
		changed = false;
		for(auto& it : rs.rules) {
			std::string name = it.first;
			auto s = rs.first.find(name);
			if(s == rs.first.end()) {
				if(rs.rules.find(it.second.rule.front().name) == rs.rules.end()) {
					std::set<std::string> n;
					n.insert(it.second.rule.front().name);
					auto ret = rs.first.insert(std::make_pair(
						name,
						n
					));
	
					changed |= ret.second;
				} else {
					auto o = rs.first.find(it.second.rule.front().name);
					std::set<std::string> n;
					if(o != rs.first.end()) {
						for(auto& jt : o->second) {
							n.insert(jt);
						}
					}
					auto ret = rs.first.insert(std::make_pair(
						name,
						n
					));
					changed |= ret.second;
				}
			} else {
				if(rs.rules.find(it.second.rule.front().name) == rs.rules.end()) {
					s->second.insert(it.second.rule.front().name);
				} else {
					auto o = rs.first.find(it.second.rule.front().name);
					if(o != rs.first.end()) {
						for(auto& jt : o->second) {
							changed |= s->second.insert(jt).second;
						}
					}
				}
			}
		}
	} while(changed);
}

std::vector<RulePart> getNameVector(const Expr& e) {
	std::vector<RulePart> ret;
	for(auto& it : e.rule) {
		ret.push_back(it);
	}
	return ret;
}

void RecurDec::genRules() {
	auto start = rs.rules.find("Start");
	ASSERT_T(start != rs.rules.end());
	genRules(start->first);
}

void RecurDec::walkTrie(const GrammarPrefix::TrieEntry& path, const size_t depth) {

}

void RecurDec::genRules(const std::string& start) {
	allreadyDone.insert(start);

	// header
	const std::string headerStringTest("\tbool lookAheadTest%s(const Token&);\n");
	const std::string headerStringParse("\t%sPtr parse%s();\n");

	format(headerS, headerStringTest, start);
	format(headerS, headerStringParse, start, start);


	// source
	const std::string srcStringTest("\tbool Parser::lookAheadTest%s(const Token& token) {\n");
	const std::string srcStringParse("\t%sPtr Parser::parse%s() {\n");

	format(srcS, srcStringTest, start);
	format(srcS, "\t\treturn\n");
	auto lookAheadSet = rs.first.find(start);
	ASSERT_T(lookAheadSet != rs.first.end());
	const size_t lookAheadSetSize = lookAheadSet->second.size();
	size_t i = 0;
	for(auto& it : lookAheadSet->second) {
		format(srcS, "\t\t\ttoken.type == %s", it);
		if(i+1 == lookAheadSetSize) {
			format(srcS, ";\n");
		} else {
			format(srcS, " ||\n");
		}
		++i;
	}
	format(srcS, "\t}\n\n");

	GrammarPrefix trie;
	auto range = rs.rules.equal_range(start);
	for(; range.first != range.second; ++range.first) {
		auto vec = getNameVector(range.first->second);
		trie.insert(vec.begin(), vec.end(), true);
	}

	format(srcS, srcStringParse, start, start);
	format(srcS, "\tauto curToken = textToken();\n");

	std::cout<<trie<<std::endl;
}
