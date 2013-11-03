#include <recdec.hpp>
#include <unit.hpp>

ErrorStuff::ErrorStuff(const std::string& r, const std::string& p, const size_t d,
		const std::vector<std::string>& po) : rule(r), part(p), depth(d), possible(po)
{
} 

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

void RecurDec::genAstForwardDecl() {
	format(out.astH, "pragma once\n\n");
	format(out.astH,  "import <token.hpp>\n\n");
	std::set<std::string> done;
	for(auto& it : rs.rules) {
		if(done.find(it.first) != done.end()) {
			continue;
		}
		done.insert(it.first);
		format(out.astH, "class %s;\n", it.first);
		format(out.astH, "typedef std::shared_ptr<%s> %sPtr;\n", it.first, it.first);
	}
}

size_t RecurDec::newErrorStuff(const std::string& rule, const std::string& part, const size_t depth,
		const std::vector<std::string>& p) {
	errorStuff.emplace_back(rule, part, depth, p);
	return errorStuff.size() - 1u;
}

void RecurDec::writeErrorStuff() {
	format(out.errH, "pragma once\n\n");
	format(out.errH, "#include <vector>\n\n");
	format(out.errH, "#include <token.hpp>\n\n");
	format(out.errH, "class ParseException : public std::exception {\n");
	format(out.errH, "public:\n");
	format(out.errH, "\tconst size_t id;\n");
	format(out.errH, "\tconst Token token;\n");
	format(out.errH, "\tParseException(const Token&, const size_t);\n");
	format(out.errH, "};\n\n");

	format(out.errH, "struct ErrorType {\n");
	format(out.errH, "\tconst size_t id;\n");
	format(out.errH, "\tconst std::string rule;\n");
	format(out.errH, "\tconst std::string part;\n");
	format(out.errH, "\tconst size_t depth;\n");
	format(out.errH, "\tconst std::vector<std::string> follow;\n");
	format(out.errH, "\tErrorType(const size_t,const std::string&,const std::string&,const size_t,std::initializer_list<std::string>);\n");
	format(out.errH, "};\n\n");
	format(out.errH, "const ErrorType& getError(const size);\n\n");
	format(out.errH, "typedef std::vector<const ErrorType> ErrorTypeVector;\n\n");
	format(out.errH, "const ErrorTypeVector vectorOfError = {\n");
	for(size_t i = 0; i < errorStuff.size(); ++i) {
		format(out.errH, "\tErrorType(%u, \"%s\", \"%s\", %u, {\n", i, errorStuff[i].rule, 
			errorStuff[i].part, errorStuff[i].depth
		);
		for(size_t j = 0; j < errorStuff[i].possible.size(); ++j) {
			format(out.errH, "\t\t\"%s\"%s", errorStuff[i].possible[j],
				j+1 == errorStuff[i].possible.size() ? "\n" : ",\n"
			);
		}
		format(out.errH, "\t})%s", i+1 == errorStuff.size() ? "\n" : ";\n");
	}
	format(out.errH, "};\n");

	format(out.errS, "#include <%s>\n\n", out.errHfn);
	format(out.errH, "ParseException::ParseException(const Token& t, const size_t i) : token(t), id(i) {}\n\n");
	format(out.errS, "ErrorType::ErrorType(const size_t i,const std::string& r, const std::string& p, const size_t d, \n\tstd::initializer_list<std::string> f) : \n");
	format(out.errS, "\tid(i), rule(r), part(p), depth(d), follow(f) {\n");
	format(out.errS, "}\n\n");
}

void RecurDec::alkTrie(const GrammarPrefix::TrieEntry* path, const std::string& part, const size_t depth) {
	std::string prefix(depth, '\t');
	bool wasFirst = false;
	if(path->map.empty()) {
		return;
	}
	for(auto& it : path->map) {
		bool pushed = false;
		format(out.prsS, "%s%s(lookAheadTest%s(curToken)) {\n", 
			wasFirst ? "" : prefix, 
			wasFirst ? " else if" : "if", it.first.name
		);
		if(rs.token.find(it.first.name) != rs.token.end() && 
				!it.first.storeName.empty()) {
			nameStack.push_back(it.first.storeName);
			pushed = true;
			format(out.prsS, "%s\tToken %s(curToken);\n",prefix, 
				it.first.storeName
			);
			format(out.prsS, "%s\tnextToken();\n", prefix);
		} else if(rs.rules.find(it.first.name) != rs.rules.end() && 
				!it.first.storeName.empty()) {
			nameStack.push_back(it.first.storeName);
			pushed = true;
			format(out.prsS, "%s\t%sPtr %s(parse());\n", prefix, it.first.name, 
				it.first.storeName
			);
		} else {
			format(out.prsS, "%s\tnextToken();\n", prefix);
		}
		if(it.second.isValue) {
			format(out.prsS, "\t%sreturn std::make_shared<%s>(", prefix, 
				current
			);
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
	std::vector<std::string> possible;
	for(auto& it : path->map) {
		possible.push_back(it.first.name);
	}
		
	format(out.prsS, " else {\n");
	format(out.prsS, "%s\tthrow ParseException(curToken, %u", prefix,
			newErrorStuff(current, part, depth, possible)
	);
	/*size_t cnt = 0;
	for(auto& it : path->map) {
		format(out.prsS, "%s\t\tTokenType::%s%s", prefix, it.first.name, 
			cnt+1 == path->map.size() ? "\n" : ",\n"
		);
		++cnt;
	}*/
	format(out.prsS, ");\n");
	//format(out.prsS, "%s\t);\n", prefix);
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

	walkTrieConstructor(&trie.getRoot(), std::vector<std::string>());

	//std::cout<<trie<<std::endl;
}

void RecurDec::walkTrieConstructor(const GrammarPrefix::TrieEntry* path, 
		std::vector<std::string> cur) {
	if(path->isValue) {
		this->store.push_back(cur);
	}

	for(auto& it : path->map) {
	}

}

void RecurDec::gen() {
	this->genRules();
	this->writeErrorStuff();
	this->genAstForwardDecl();
}
