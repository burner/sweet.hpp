#include <recdec.hpp>
#include <unit.hpp>

RecurDec::RecurDec(RuleStore& r, Output& o) : out(o), rs(r) {
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
	//current = "VarDecl";
	//auto start = rs.rules.find(current);
	//ASSERT_T(start != rs.rules.end());
	//genRules(start->first);
	std::set<std::string> done;
	for(auto& it : rs.rules) {
		if(done.find(it.first) != done.end()) {
			continue;
		}
		current = it.first;
		genRules(it.first);
		done.insert(it.first);
	}
}

void RecurDec::walkTrie(const GrammarPrefix::TrieEntry* path, const std::string& know, const size_t depth) {
	std::string prefix(depth, '\t');
	bool wasFirst = false;
	if(path->map.empty()) {
		return;
	}
	for(auto& it : path->map) {
		bool pushed = false;
		format(out.prsS, "%s%s(lookAheadTest%s(cur)) {\n", wasFirst ? "" : prefix, 
			wasFirst ? " else if" : "if", it.first.name
		);
		if(rs.token.find(it.first.name) != rs.token.end() && !it.first.storeName.empty()) {
			nameStack.push_back(it.first.storeName);
			pushed = true;
			format(out.prsS, "%s\tToken %s(curToken);\n",prefix, it.first.storeName);
			format(out.prsS, "%s\tnextToken();\n", prefix);
		} else if(rs.rules.find(it.first.name) != rs.rules.end() && !it.first.storeName.empty()) {
			nameStack.push_back(it.first.storeName);
			pushed = true;
			format(out.prsS, "%s\t%sPtr %s(parse());\n", prefix, it.first.name, it.first.storeName);
		} else {
			format(out.prsS, "%s\tnextToken();\n", prefix);
		}
		if(it.second.isValue) {
			format(out.prsS, "\t%sreturn std::make_shared<%s>(", prefix, current);
			size_t idx = 0;
			for(auto& name : nameStack) {
				format(out.prsS, "%s", name);
				if(idx+1 < nameStack.size()) {
					format(out.prsS, ", ");
				}
				++idx;
			}
			format(out.prsS, ");\n");
		}
		walkTrie(&(it.second), it.first.name, depth+1);
		if(pushed) {
			nameStack.pop_back();
		}
		format(out.prsS, "%s}", prefix);
		wasFirst = true;
	}
	format(out.prsS, " else {\n");
	format(out.prsS, "%s\tthrow ParseException(%s, %s, %d,\n%s\t\t{\n", prefix,
			current, know, depth, prefix
	);
	size_t cnt = 0;
	for(auto& it : path->map) {
		format(out.prsS, "%s\t\t\t\"%s\"%s", prefix, it.first.name, cnt+1 == path->map.size() ? "\n" : ",\n");
		++cnt;
	}
	format(out.prsS, "%s\t\t}\n", prefix);
	format(out.prsS, "%s\t);\n", prefix);
	format(out.prsS, "%s}", prefix);
	format(out.prsS, "\n");
}

void RecurDec::genRules(const std::string& start) {
	allreadyDone.insert(start);

	// header
	const std::string headerStringTest("bool lookAheadTest%s(const Token&);\n");
	const std::string headerStringParse("%sPtr parse%s();\n");

	format(out.prsH, headerStringTest, start);
	format(out.prsH, headerStringParse, start, start);


	// source
	const std::string srcStringTest("bool Parser::lookAheadTest%s(const Token& token) {\n");
	const std::string srcStringParse("%sPtr Parser::parse%s() {\n");

	format(out.prsS, srcStringTest, start);
	format(out.prsS, "\treturn");
	auto lookAheadSet = rs.first.find(start);
	ASSERT_T(lookAheadSet != rs.first.end());
	const size_t lookAheadSetSize = lookAheadSet->second.size();
	size_t i = 0;
	for(auto& it : lookAheadSet->second) {
		format(out.prsS, "%stoken.type == TokenType::%s", (i != 0 ? "\t\t" : " "), it);
		if(i+1 == lookAheadSetSize) {
			format(out.prsS, ";\n");
		} else {
			format(out.prsS, " ||\n");
		}
		++i;
	}
	format(out.prsS, "}\n\n");

	GrammarPrefix trie;
	auto range = rs.rules.equal_range(start);
	for(; range.first != range.second; ++range.first) {
		auto vec = getNameVector(range.first->second);
		trie.insert(vec.begin(), vec.end(), true);
	}

	format(out.prsS, srcStringParse, start, start);
	walkTrie(&trie.getRoot(), start, 1);
	format(out.prsS, "}\n");

	//std::cout<<trie<<std::endl;
}

void RecurDec::walkTrieConstructor(const GrammarPrefix::TrieEntry* path, 
		std::vector<std::string> cur, std::vector<std::vector<<std::string>>& store) {

}
