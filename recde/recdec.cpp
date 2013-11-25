#include <recdec.hpp>
#include <unit.hpp>
#include <format.hpp>
#include <logger.hpp>

ErrorStuff::ErrorStuff(const std::string& r, const std::string& p, 
	const size_t d, const std::vector<std::string>& po) : rule(r), part(p), 
	depth(d), possible(po)
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
				if(rs.rules.find(it.second.rule.front().name) == 
						rs.rules.end()) 
				{
					std::set<std::string> n;
					n.insert(it.second.rule.front().name);
					auto ret = rs.first.insert(std::make_pair(name, n));
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
				if(rs.rules.find(it.second.rule.front().name) == 
					rs.rules.end()) 
				{
					s->second.insert(it.second.rule.front().name);
				} else {
					auto o = rs.first.find(it.second.rule.front().name);
					if(o != rs.first.end()) {
						for(auto& jt : o->second) {
							auto o2 = s->second.insert(jt);
							changed |= o2.second;
						}
					}
				}
			}
		}
	} while(changed);
}

std::vector<RulePart> getNameVector(const Expr& e) {
	std::vector<RulePart> ret;
	LOG();
	for(auto& it : e.rule) {
		std::cout<<it<<" ";
		ret.push_back(it);
	}
	std::cout<<std::endl;
	return ret;
}

void RecurDec::genRules() {
	//current = "VarDecl";
	//auto start = rs.rules.find(current);
	//ASSERT_T(start != rs.rules.end());
	//genRules(start->first);

	format(out.prsH, "// DO not MODIFY this FILE it IS generated\n\n");
	format(out.prsH, "#pragma once\n\n");
	format(out.prsH, "#include <ast.hpp>\n");
	format(out.prsH, "#include <token.hpp>\n");
	format(out.prsH, "#include <lexer.hpp>\n");
	format(out.prsS, "#include <lexer.hpp>\n");
	format(out.prsH, "\nclass Parser {\n");
	format(out.prsH, "public:\n");
	format(out.prsH, "\tParser(Lexer&);\n");

	format(out.prsS, "// DO not MODIFY this FILE it IS generated\n\n");
	format(out.prsS, "#include <parser.hpp>\n");
	format(out.prsS, "#include <ast.hpp>\n");
	format(out.prsS, "#include <error.hpp>\n");
	format(out.prsS, "\nParser::Parser(Lexer& l) : lexer(l) {\n");
	format(out.prsS, "\tthis->nextToken();\n");
	format(out.prsS, "}\n\n");
	format(out.prsS, "void Parser::nextToken() {\n");
	format(out.prsS, "\tthis->curToken = this->lexer.nextToken();\n");
	format(out.prsS, "}\n");

	std::set<std::string> done;
	/*for(auto& it : rs.token) {
		if(done.find(it.first) != done.end()) {
			continue;
		}

	}

	done.clear();*/
	for(auto& it : rs.rules) {
		if(done.find(it.first) != done.end()) {
			continue;
		}

		LOG("\n\n%s\n\n", it.first);
		current = it.first;
		genRules(it.first);
		done.insert(it.first);
	}
	format(out.prsH, "\nprivate:\n");
	format(out.prsH, "\tLexer& lexer;\n");
	format(out.prsH, "\tToken curToken;\n");
	format(out.prsH, "\tvoid nextToken();\n");
	format(out.prsH, "};\n");
}

void RecurDec::genAstForwardDecl() {
	format(out.mulH, "// DO not MODIFY this FILE it IS generated\n\n");
	format(out.mulH, "#pragma once\n\n");
	format(out.mulH, "#include <vector>\n");
	format(out.mulH, "#include <functional>\n\n");
	format(out.mulH, "#include <visitor.hpp>\n\n");
	format(out.mulH, "typedef std::vector<std::reference_wrapper<Visitor>>"
		" VisitorVec;\n\n");
	format(out.mulH, "class MultiVisitor : public Visitor {\n");
	format(out.mulH, "public:\n");
	format(out.mulH, "\tvoid addVisitor(Visitor&);\n");
	format(out.mulH, "\tVisitorVec& getVisitor();\n");
	format(out.mulH, "\tconst VisitorVec& getVisitor() const;\n\n");

	format(out.mulS, "#include <ast.hpp>\n");
	format(out.mulS, "#include <multivisitor.hpp>\n\n");

	format(out.mulS, "void MultiVisitor::addVisitor(Visitor& v) {\n");
	format(out.mulS, "\tthis->visitor.emplace_back(std::ref(v));\n");
	format(out.mulS, "}\n\n");

	format(out.mulS, "VisitorVec& MultiVisitor::getVisitor() {\n");
	format(out.mulS, "\treturn this->visitor;\n");
	format(out.mulS, "}\n\n");

	format(out.mulS, "const VisitorVec& MultiVisitor::getVisitor() const {\n");
	format(out.mulS, "\treturn this->visitor;\n");
	format(out.mulS, "}\n\n");

	format(out.visH, "// DO not MODIFY this FILE it IS generated\n\n");
	format(out.visH, "#pragma once\n\n");
	//format(out.visH, "#include <ast.hpp>\n\n");
	
	std::set<std::string> done;
	for(auto& it : rs.rules) {
		if(done.find(it.first) != done.end()) {
			continue;
		}
		done.insert(it.first);
		format(out.visH, "class %s;\n", it.first);
	}
	format(out.visH, "\n");

	format(out.visH, "class Visitor {\n");
	format(out.visH, "public:\n");

	format(out.outH, "// DO not MODIFY this FILE it IS generated\n\n");
	format(out.outH, "#pragma once\n\n");
	format(out.outH, "#include <ostream>\n\n");
	format(out.outH, "#include <visitor.hpp>\n\n");
	format(out.outH, "class StdOutVisitor : public Visitor {\n");
	format(out.outH, "public:\n");
	format(out.outH, "\tStdOutVisitor(std::ostream&);\n");
	format(out.outS, "#include <ast.hpp>\n");
	format(out.outS, "#include <outvisitor.hpp>\n");

	format(out.outS, "StdOutVisitor::StdOutVisitor(std::ostream& s) : "
		"ss(s), indent(0u) {\n}\n\n");
	format(out.outS, "void StdOutVisitor::incrementIndent() { ++indent; }\n");
	format(out.outS, "void StdOutVisitor::decrementIndent() { --indent; }\n");
	format(out.outS, "\nvoid StdOutVisitor::makeIndent() {\n");
	format(out.outS, "\tfor(size_t i = 0; i < this->indent; ++i) {\n");
	format(out.outS, "\t\tss<<' ';\n");
	format(out.outS, "\t}\n");
	format(out.outS, "}\n\n");

	format(out.dotH, "// DO not MODIFY this FILE it IS generated\n\n");
	format(out.dotH, "#pragma once\n\n");
	format(out.dotH, "#include <ostream>\n\n");
	format(out.outS, "#include <ast.hpp>\n");
	format(out.dotH, "#include <visitor.hpp>\n\n");
	format(out.dotH, "class DotVisitor : public Visitor {\n");
	format(out.dotH, "public:\n");
	format(out.dotH, "\tDotVisitor(std::ostream&);\n");
	format(out.dotH, "\tvirtual ~DotVisitor();\n");
	format(out.dotH, "\tvoid writeHeader(const AstNode*);\n");

	format(out.dotS, "// DO not MODIFY this FILE it IS generated\n\n");
	format(out.dotS, "#include <ast.hpp>\n");
	format(out.dotS, "#include <dotvisitor.hpp>\n\n");

	format(out.dotS, "DotVisitor::DotVisitor(std::ostream& s) : "
		"ss(s) {\n\tss<<\"digraph g {\\n\"<<std::endl;\n}\n\n");
	format(out.dotS, "DotVisitor::~DotVisitor() {\n\tss<<\"}\""
		"<<std::endl;\n}\n\n");
	format(out.dotS, "void DotVisitor::writeHeader(const AstNode* node) {\n");
	format(out.dotS, "\tss<<\"node\"<<node->getId()<<\" "
		"[style = \\\"filled\\\" penwidth = 1 fillcolor = \\\"white\\\" "
		"shape =\\\"Mrecord\\\" label=\";\n");
	format(out.dotS, "\tss<<\"<<table border=\\\"0\\\" cellborder=\\\"0\\\""
		" cellpadding=\\\"3\\\" \";\n");
	format(out.dotS, "\tss<<\"bgcolor=\\\"white\\\">\\n\";\n");
	format(out.dotS, "\tss<<\"<tr>\\n\";\n");
	format(out.dotS, "\tss<<\"\\t<td bgcolor=\\\"black\\\" align=\\\""
		"center\\\" colspan=\\\"2\\\">\\n\";\n");
	format(out.dotS, "\tss<<\"\\t<font color=\\\"white\\\">\";\n");
	format(out.dotS, "}\n\n");

	format(out.astH, "// DO not MODIFY this FILE it IS generated\n\n");
	format(out.astH, "#pragma once\n\n");
	format(out.astH, "#include <cstdint>\n\n");
	format(out.astH, "#include <memory>\n");
	format(out.astH, "#include <ostream>\n");
	format(out.astH, "#include <string>\n");
	format(out.astH, "#include <token.hpp>\n\n");
	format(out.astH, "#include <visitor.hpp>\n\n");
	format(out.astH, "class SymbolTable;\n\n");
	format(out.astH, "uint64_t getNextAstId();\n\n");
	format(out.astH, "class AstNode {\n"); 
	format(out.astH, "public:\n"); 
	format(out.astH, "\tAstNode();\n"); 
	format(out.astH, "\tuint64_t getId() const;\n\n"); 
	format(out.astH, "\tAstNode* getParent();\n"); 
	format(out.astH, "\tconst AstNode* getParent() const;\n"); 
	format(out.astH, "\tvoid setParent(AstNode*);\n\n"); 
	format(out.astH, "\tSymbolTable* getSymbolTable();\n"); 
	format(out.astH, "\tconst SymbolTable* getSymbolTable() const;\n"); 
	format(out.astH, "\tvoid setSymbolTable(SymbolTable*);\n"); 
	format(out.astH, "\nprivate:\n"); 
	format(out.astH, "\tSymbolTable* symbolTable;\n");
	format(out.astH, "\tuint64_t id;\n");
	format(out.astH, "\tAstNode* parent;\n");
	format(out.astH, "};\n\n"); 
	format(out.astH,  "\n// Forward Decl\n\n");
	done.clear();
	for(auto& it : rs.rules) {
		if(done.find(it.first) != done.end()) {
			continue;
		}
		done.insert(it.first);
		format(out.astH, "class %s;\n", it.first);
		format(out.astH, "typedef std::shared_ptr<%s> %sPtr;\n", it.first, 
			it.first
		);
		format(out.astH, "typedef std::shared_ptr<const %s> %sConstPtr;\n", 
			it.first, it.first
		);

		// Visitor header
		format(out.visH, "\tvirtual bool visit%s(%s*) = 0;\n\n", 
			it.first, it.first
		);
		format(out.visH, "\tvirtual bool visit%s(const %s*) = 0;\n\n", 
			it.first, it.first
		);
		format(out.visH, "\tvirtual bool leave%s(%s*) = 0;\n", it.first, 
			it.first
		);
		format(out.visH, "\tvirtual bool leave%s(const %s*) = 0;\n\n", 
			it.first, it.first
		);

		// Include method decl
		format(out.inH, "\tvirtual bool visit%s(%s*) override;\n", it.first, 
			it.first
		);
		format(out.inH, "\tvirtual bool visit%s(const %s*) override;\n\n", 
			it.first, it.first
		);
		format(out.inH, "\tvirtual bool leave%s(%s*) override;\n", it.first, 
			it.first
		);
		format(out.inH, "\tvirtual bool leave%s(const %s*) override;\n", 
			it.first, it.first
		);

		// Multi Visitor
		format(out.mulH, "\tvirtual bool leave%s(%s*) override;\n", it.first, 
			it.first
		);
		format(out.mulH, "\tvirtual bool leave%s(const %s*) override;\n\n", 
			it.first, it.first
		);
		format(out.mulH, "\tvirtual bool visit%s(%s*) override;\n", it.first, 
			it.first
		);
		format(out.mulH, "\tvirtual bool visit%s(const %s*) override;\n\n", 
			it.first, it.first
		);

		// StdOut Visitor
		format(out.outH, "\tvirtual bool visit%s(%s*) override;\n", it.first, 
			it.first);
		format(out.outH, "\tvirtual bool visit%s(const %s*) override;\n\n", 
			it.first, it.first
		);
		format(out.outH, "\tvirtual bool leave%s(%s*) override;\n", it.first, 
			it.first);
		format(out.outH, "\tvirtual bool leave%s(const %s*) override;\n\n", 
			it.first, it.first
		);

		// Dot Visitor
		format(out.dotH, "\tvirtual bool visit%s(%s*) override;\n", it.first, 
			it.first);
		format(out.dotH, "\tvirtual bool visit%s(const %s*) override;\n\n", 
			it.first, it.first
		);
		format(out.dotH, "\tvirtual bool leave%s(%s*) override;\n", it.first, 
			it.first);
		format(out.dotH, "\tvirtual bool leave%s(const %s*) override;\n\n", 
			it.first, it.first
		);
	}
	format(out.visH, "};\n");
	format(out.outH, "private:\n");
	format(out.outH, "\tstd::ostream& ss;\n");
	format(out.outH, "\tsize_t indent;\n\n");
	format(out.outH, "\tvoid makeIndent();\n");
	format(out.outH, "\tvoid incrementIndent();\n");
	format(out.outH, "\tvoid decrementIndent();\n");
	format(out.outH, "};\n");

	format(out.mulH, "private:\n");
	format(out.mulH, "\tVisitorVec visitor;\n");
	format(out.mulH, "};\n");

	format(out.dotH, "private:\n");
	format(out.dotH, "\tstd::ostream& ss;\n");
	format(out.dotH, "};\n");

	format(out.astH,  "\n\n// Decl");
	format(out.astH, "\n");

	format(out.astS, "// DO not MODIFY this FILE it IS generated\n\n");
	format(out.astS, "#include <ast.hpp>\n\n");
	format(out.astS, "uint64_t getNextAstId() {\n\tstatic uint64_t globalId;\n\treturn globalId++;\n}\n\n");
	format(out.astS, "AstNode::AstNode() : id(getNextAstId()), parent(nullptr) {\n}\n\n");
	format(out.astS, "uint64_t AstNode::getId() const {\n\treturn this->id;\n}\n\n");
	format(out.astS, "const AstNode* AstNode::getParent() const {\n\treturn this->parent;\n}\n\n");
	format(out.astS, "AstNode* AstNode::getParent() {\n\treturn this->parent;\n}\n\n");
	format(out.astS, "void AstNode::setParent(AstNode* p) {\n\tthis->parent = p;\n}\n\n");
	format(out.astS, "const SymbolTable* AstNode::getSymbolTable() const "
		"{\n\treturn this->symbolTable;\n}\n\n");
	format(out.astS, "SymbolTable* AstNode::getSymbolTable() {\n\treturn "
		"this->symbolTable;\n}\n\n");
	format(out.astS, "void AstNode::setSymbolTable(SymbolTable* t) "
		"{\n\tthis->symbolTable = t;\n}\n\n");
	//format(out.astS, "std::ostream& AstNode::operator<<(std::ostream& ss) const "
	//	"{\n\ttoOutStream(ss);\n\treturn ss;\n}\n\n"
	//);
}

size_t RecurDec::newErrorStuff(const std::string& rule, const std::string& part, const size_t depth,
		const std::vector<std::string>& p) {
	errorStuff.emplace_back(rule, part, depth, p);
	return errorStuff.size() - 1u;
}

void RecurDec::writeErrorStuff() {
	format(out.errH, "#pragma once\n\n");
	format(out.errH, "#include <vector>\n\n");
	format(out.errH, "#include <token.hpp>\n");
	format(out.errH, "#include <ast.hpp>\n\n");
	format(out.errH, "class ParseException : public std::exception {\n");
	format(out.errH, "public:\n");
	format(out.errH, "\tconst size_t id;\n");
	format(out.errH, "\tconst Token token;\n\n");
	format(out.errH, "\tParseException(const Token&, const size_t);\n");
	format(out.errH, "\tconst char* what() const throw();\n");
	format(out.errH, "};\n\n");

	format(out.errH, "struct ErrorType {\n");
	format(out.errH, "\tconst size_t id;\n");
	format(out.errH, "\tconst std::string rule;\n");
	format(out.errH, "\tconst std::string part;\n");
	format(out.errH, "\tconst size_t depth;\n");
	format(out.errH, "\tconst std::vector<std::string> follow;\n");
	format(out.errH, "\tErrorType(const size_t,const std::string&,const std::string&,const size_t,"
		"std::initializer_list<std::string>);\n"
	);
	format(out.errH, "};\n\n");
	format(out.errH, "const ErrorType& getError(const size_t);\n\n");
	format(out.errH, "typedef std::vector<ErrorType> ErrorTypeVector;\n\n");
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
		format(out.errH, "\t})%s", i+1 == errorStuff.size() ? "\n" : ",\n");
	}
	format(out.errH, "};\n");

	format(out.errS, "#include <string>\n");
	format(out.errS, "#include <sstream>\n\n");
	format(out.errS, "#include <error.hpp>\n\n");
	format(out.errS, "ParseException::ParseException(const Token& t, const size_t i) : "
		"id(i), token(t) {}\n\n"
	);
	format(out.errS, "const char* ParseException::what() const throw() {\n");
	format(out.errS, "\tstd::stringstream ss;\n\tss<<this->token<<\" \"<<this->id;\n");
	format(out.errS, "\treturn ss.str().c_str();\n}\n\n");
	format(out.errS, "ErrorType::ErrorType(const size_t i,const std::string& r, const std::string& p, "
		"const size_t d, \n\tstd::initializer_list<std::string> f) : \n"
	);
	format(out.errS, "\tid(i), rule(r), part(p), depth(d), follow(f) {\n");
	format(out.errS, "}\n");
}

void RecurDec::walkTrie(const GrammarPrefix::TrieEntry* path, const std::string& part, const size_t depth) {
	std::string prefix(depth, '\t');
	bool wasFirst = false;
	if(path->map.empty()) {
		return;
	}
	for(auto& it : path->map) {
		bool pushed = false;
		if(rs.rules.find(it.first.name) != rs.rules.end()) {
			format(out.prsS, "%s%s(lookAheadTest%s(curToken)) {\n", 
				wasFirst ? "" : prefix, 
				wasFirst ? " else if" : "if", it.first.name
			);
		} else {
			format(out.prsS, "%s%s(curToken.type == TokenType::%s) {\n", 
				wasFirst ? "" : prefix, 
				wasFirst ? " else if" : "if", it.first.name
			);

		}
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
			format(out.prsS, "%s\t%sPtr %s(parse%s());\n", prefix, 
				it.first.name, it.first.storeName, it.first.name
			);
		} else {
			format(out.prsS, "%s\tnextToken();\n", prefix);
		}
		walkTrie(&(it.second), it.first.name, depth+1);
		if(it.second.isValue) {
			format(out.prsS, "\t%sreturn std::make_shared<%s>(", prefix, 
				current
			);
			for(auto& name : nameStack) {
				format(out.prsS, "%s, ", name);
			}
			format(out.prsS, "\n%s\t\t%sEnum::%s\n", prefix, current, 
				it.second.value);
			format(out.prsS, "\t%s);\n", prefix);
		}
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
		
	if(!path->isValue) {
		format(out.prsS, " else {\n");
		format(out.prsS, "%s\tthrow ParseException(curToken, %u", prefix,
				newErrorStuff(current, part, depth, possible)
		);
		format(out.prsS, ");\n");
		format(out.prsS, "%s}", prefix);
	}	
	format(out.prsS, "\n");
}

void RecurDec::genRules(const std::string& start) {
	allreadyDone.insert(start);

	// header
	const std::string headerStringTest(
		"\tbool lookAheadTest%s(const Token&);\n");
	const std::string headerStringParse("\t%sPtr parse%s();\n");

	format(out.prsH, "\n\n\t// %s\n", current);
	format(out.prsH, headerStringTest, start);
	format(out.prsH, headerStringParse, start, start);


	// source
	const std::string srcStringTest(
		"bool Parser::lookAheadTest%s(const Token& token) {\n");
	const std::string srcStringParse("%sPtr Parser::parse%s() {\n");

	format(out.prsS, "\n\n// %s\n\n", current);
	format(out.prsS, srcStringTest, start);
	format(out.prsS, "\treturn");
	auto lookAheadSet = rs.first.find(start);
	ASSERT_T(lookAheadSet != rs.first.end());
	const size_t lookAheadSetSize = lookAheadSet->second.size();
	size_t i = 0;
	for(auto& it : lookAheadSet->second) {
		format(out.prsS, "%stoken.type == TokenType::%s", 
			(i != 0 ? "\t\t" : " "), it);
		if(i+1 == lookAheadSetSize) {
			format(out.prsS, ";\n");
		} else {
			format(out.prsS, " ||\n");
		}
		++i;
	}
	format(out.prsS, "}\n\n");

	GrammarPrefix trie;
	GrammarPrefixEnum trieEnum;
	auto range = rs.rules.equal_range(start);
	for(; range.first != range.second; ++range.first) {
		std::string endName = range.first->second.rule[0].endName;
		auto vec = getNameVector(range.first->second);
		trie.insert(vec.begin(), vec.end(), endName);
		trieEnum.insert(vec.begin(), vec.end(), endName);
	}

	format(out.prsS, srcStringParse, start, start);
	walkTrie(&trie.getRoot(), start, 1);
	LOG("The trie");
	std::cout<<trie<<std::endl;
	std::cout<<trieEnum<<std::endl;
	format(out.prsS, "}\n");

	this->store.clear();
	this->store2.clear();
	walkTrieConstructor(&trie.getRoot(), std::vector<RulePart>());
	walkTrieConstructor2(&trieEnum.getRoot(), std::vector<RulePart>());
	for(auto& it : this->store) {
		LOG();
		for(auto& jt : it) {
			format(std::cout, "%s ", jt);
		}
		std::cout<<std::endl;
	}
	this->genAst(this->store);

}

void RecurDec::walkTrieConstructor2(const GrammarPrefixEnum::TrieEntry* path, 
		std::vector<RulePart> cur) {
	if(path->isValue) {
		this->store2.push_back(cur);
	}

	for(auto& it : path->map) {
		std::vector<RulePart> n(cur.begin(), cur.end());
		if(!it.first.storeName.empty()) {
			n.push_back(it.first);
		}
		walkTrieConstructor2(&(it.second), n);
	}
}

void RecurDec::walkTrieConstructor(const GrammarPrefix::TrieEntry* path, 
		std::vector<RulePart> cur) {
	if(path->isValue) {
		this->store.push_back(cur);
	}

	for(auto& it : path->map) {
		std::vector<RulePart> n(cur.begin(), cur.end());
		if(!it.first.storeName.empty()) {
			n.push_back(it.first);
		}
		walkTrieConstructor(&(it.second), n);
	}
}

void RecurDec::genAstClassDeclStart() {
	format(out.astH, "class %s : public AstNode {\n", current);
	format(out.astH, "public:\n");
}

/*
	format(out.astS, "\tconst std::string indent(depth, ' ');\n");
	format(out.astS, "\tss<<indent<<\"%s\"<<\" id(\"<<", current);
	out.astS<<"this->getId()"<<"<<\") rule(\"<<this->rule<<\")\"<<std::endl;\n";
					//format(out.astS, "\t\tthis->%s.toOutStream(ss, depth+1);\n", jt.storeName);
*/

void RecurDec::genVisitor(
		const std::map<std::string,std::vector<RulePart>>& rules) 
{
	format(out.astS, "void %s::acceptVisitor(Visitor& visitor) const {\n", 
		current);
	format(out.astS, "\tconst %s* tmp = this;\n", current);
	format(out.astS, "\tvisitor.visit%s(tmp);\n", current);
	bool first = true;
	size_t globalCnt = 0;
	for(auto& it : rules) {
		size_t cnt = 0;
		for(auto& jt : it.second) {
			if(!jt.storeName.empty()) {
				if(rs.rules.find(jt.name) != rs.rules.end()) {
					++cnt;
				}
			}
		}
		LOG("cnt %u", cnt);
		if(!cnt) {
			continue;
		}
		globalCnt += cnt;

		format(out.astS, "\t%s(this->rule == %sEnum::%s) {\n", 
			first ? "if" : "} else if", current, it.first
		);
		first = false;
		LOG("%s", it.second);
		for(auto& jt : it.second) {
			if(!jt.storeName.empty()) {
				if(rs.rules.find(jt.name) != rs.rules.end()) {
					format(out.astS, "\t\tthis->%s->acceptVisitor(visitor);\n",
						jt.storeName
					);
				} 
			}
		}
	}
	//if(!rules.empty()) {
	if(globalCnt) {
		format(out.astS, "\t}\n\n");
	}

	format(out.astS, "\tvisitor.leave%s(tmp);\n", current);
	format(out.astS, "}\n");
	format(out.astS, "void %s::acceptVisitor(Visitor& visitor) {\n", current);
	format(out.astS, "\t%s* tmp = this;\n", current);
	format(out.astS, "\tvisitor.visit%s(tmp);\n", current);

	first = true;
	globalCnt = 0;
	for(auto& it : rules) {
		size_t cnt = 0;
		for(auto& jt : it.second) {
			if(!jt.storeName.empty()) {
				if(rs.rules.find(jt.name) != rs.rules.end()) {
					++cnt;
				}
			}
		}
		LOG("cnt %u", cnt);
		if(!cnt) {
			continue;
		}
		globalCnt += cnt;

		format(out.astS, "\t%s(this->rule == %sEnum::%s) {\n", 
			first ? "if" : "} else if", current, it.first
		);
		first = false;
		LOG("%s", it.second);
		for(auto& jt : it.second) {
			if(!jt.storeName.empty()) {
				if(rs.rules.find(jt.name) != rs.rules.end()) {
					format(out.astS, "\t\tthis->%s->acceptVisitor(visitor);\n",
						jt.storeName
					);
				} 
			}
		}
	}
	//if(!rules.empty()) {
	if(globalCnt) {
		format(out.astS, "\t}\n");
	}
	format(out.astS, "\tvisitor.leave%s(tmp);\n", current);
	format(out.astS, "}\n");
}

void RecurDec::genAstClassDeclEnd(const std::set<RulePart>& allValues) {
	std::set<std::string> allreadyProcessed;
	for(auto& it : allValues) {
		std::string name = it.storeName;
		name[0] = toupper(name[0]);
		if(allreadyProcessed.count(name)) {
			continue;
		}
		if(rs.token.find(it.name) != rs.token.end()) {
			format(out.astH, "\n\tToken& get%s();\n", name);
			format(out.astH, "\tconst Token& get%s() const;\n", name);

			format(out.astS, "\nToken& %s::get%s() {\n", current, name);
			format(out.astS, "\treturn this->%s;\n}\n\n", it.storeName);
			format(out.astS, "const Token& %s::get%s() const {\n", current, 
				name);
			format(out.astS, "\treturn this->%s;\n}\n\n", it.storeName);
		} else {
			format(out.astH, "\n\t%sPtr get%s();\n", it.name, name);
			format(out.astH, "\t%sConstPtr get%s() const;\n", it.name, name);

			format(out.astS, "\n%sPtr %s::get%s() {\n", it.name, current, name);
			format(out.astS, "\treturn this->%s;\n}\n\n", it.storeName);
			format(out.astS, "%sConstPtr %s::get%s() const {\n", it.name, 
				current, name);
			format(out.astS, "\treturn this->%s;\n}\n", it.storeName);
		}
		allreadyProcessed.insert(name);
	}
	format(out.astH, "\n\t%sEnum getRule() const;\n\n", current);
	format(out.astH, "\n\tvoid acceptVisitor(Visitor&) const;\n");
	format(out.astH, "\n\tvoid acceptVisitor(Visitor&);\n\n");

	format(out.astH, "private:\n");
	std::map<std::string,std::string> names;
	for(auto& it : allValues) {
		LOG("allValues | %s %s", it.storeName, it.name);
		names.insert(std::make_pair(it.storeName,it.name));
	}

	for(auto& it : names) {
		//if(rs.token.find(it.name) != rs.token.end()) {
		LOG(" it = %s", it.first);
		if(rs.token.find(it.second) != rs.token.end()) {
			//format(out.astH, "\tToken %s;\n", it.storeName);
			format(out.astH, "\tToken %s;\n", it.first);
		} else if(rs.rules.count(it.second)) {
			//format(out.astH, "\t%sPtr %s;\n", it.name, it.storeName);
			format(out.astH, "\t%sPtr %s;\n", it.second, it.first);
		} else {
			ASSERT_T_MSG(false, "nor token neither rule");
		}
	}
	format(out.astH, "\t%sEnum rule;\n", current);
	format(out.astH, "};\n");
}

std::ostream& operator<<(std::ostream& ss, const std::vector<RulePart>& r) {
	for(auto& it : r) {
		ss<<it<<" ";
	}
	return ss;
}

std::ostream& operator<<(std::ostream& ss, const std::vector<std::string>& r) {
	for(auto& it : r) {
		ss<<it<<" ";
	}
	return ss;
}

static std::string cap(const std::string& in) {
	std::string r(in);
	if(!r.empty()) {
		r[0] = toupper(r[0]);
	}
	return r;
}

void RecurDec::genAst(const std::vector<std::vector<RulePart>>& r) {
	format(out.astH, "\n\n// %s\n\n", current);
	format(out.astS, "\n// %s\n\n", current);
	std::set<RulePart> allToStore;
	std::map<std::string,std::vector<RulePart>> enumNames;
	std::map<std::string,std::vector<RulePart>> enumNames2;
	std::set<std::string> moreEnumNames;
	auto range = rs.rules.equal_range(current);
	for(; range.first != range.second; range.first++) {
		for(auto jt : range.first->second.rule) {
			if(!jt.storeName.empty()) {
				allToStore.insert(jt);
			}
			moreEnumNames.insert(jt.endName);
		}
	}
	std::vector<std::vector<RulePart>> minimized;
	size_t cnt = 0;
	for(auto& it : r) {
		++cnt;
		bool equal = false;
		LOG("%s\n", it);
		for(auto& jt : minimized) {
			if(it.size() != jt.size()) {
				equal = false;
				break;
			} 
			for(size_t i = 0; i < it.size(); ++i) {
				if(rs.token.find(it[i].name) != rs.token.end() && 
						rs.token.find(jt[i].name) != rs.token.end()) {
					equal = true;
					continue;
				}
				if(it[i].name != jt[i].name) {
					equal = false;
					goto end;
				}
			}
		}
		end:
		LOG("equal = %b |", equal);
		if(it.empty()) {
			continue;
		}
		//for(auto& jt : it) {
			LOG("%s | %s %u", it[0].endName, it, it.size());
			enumNames.insert(std::make_pair(it[0].endName, it));
			//enumNames2.insert(std::make_pair(it[0].endName, cnt-1));
		//}
		std::cout<<std::endl;
		if(!equal) {
			LOG("INSERT %s", it);
			minimized.push_back(it);
		}
	}
	std::vector<std::vector<RulePart>> minimized2;
	cnt = 0;
	for(auto& it : this->store2) {
		++cnt;
		bool equal = false;
		LOG("%s\n", it);
		for(auto& jt : minimized2) {
			if(it.size() != jt.size()) {
				equal = false;
				break;
			} 
			for(size_t i = 0; i < it.size(); ++i) {
				if(rs.token.find(it[i].name) != rs.token.end() && 
						rs.token.find(jt[i].name) != rs.token.end()) {
					equal = true;
					continue;
				}
				if(it[i].name != jt[i].name) {
					equal = false;
					goto end2;
				}
			}
		}
		end2:
		LOG("equal = %b |", equal);
		if(it.empty()) {
			continue;
		}
		//for(auto& jt : it) {
		LOG("%s | %s %u", it[0].endName, it, it.size());
		//enumNames.insert(std::make_pair(it[0].endName, it));
		enumNames2.insert(std::make_pair(it[0].endName, it));
		//}
		/*std::cout<<std::endl;
		if(!equal) {
			LOG("INSERT %s", it);
			minimized.push_back(it);
		}*/
	}
	std::vector<std::vector<std::string>> dupCon;
	for(auto& it : minimized) {
		if(it.empty()) {
			continue;
		}
		//allToStore.insert(it.begin(), it.end());
		std::vector<std::string> myCon;
		for(auto& jt : it) {
			if(rs.token.find(jt.name) != rs.token.end()) {
				myCon.push_back(format("const Token&"));
			} else if(rs.rules.find(jt.name) != rs.rules.end()) {
				myCon.push_back(format("%sPtr", jt.name));
			}
		}
		bool allreadyThere = false;
		for(auto& jDup : dupCon) {
			if(jDup.size() != myCon.size()) {
				continue;
			}
			for(size_t i = 0; i < myCon.size(); ++i) {
				if(jDup[i] != myCon[i]) {
					goto cont;
				}
			}
			allreadyThere = true;
			cont: continue;
		}
		if(allreadyThere) {
			continue;
		}
		dupCon.push_back(myCon);
		format(out.astS, "%s::%s(", this->current, this->current);
		/*const size_t itSize = it.size();
		size_t idx = 0;*/
		for(auto& jt : it) {
			if(rs.token.find(jt.name) != rs.token.end()) {
				format(out.astS, "const Token& %s_arg%s", jt.storeName, ", ");
			} else if(rs.rules.find(jt.name) != rs.rules.end()) {
				format(out.astS, "%sPtr %s_arg%s", jt.name, jt.storeName, 
					", "
				);
			} else {
				//ASSERT_T_MSG(false, "Not reachable");
			}
			//++idx;
		}
		format(out.astS, "const %sEnum enumType)\n\t: ", current);

		std::vector<RulePart> sorted(it.begin(), it.end());
		std::sort(sorted.begin(), sorted.end(), 
			[](const RulePart& a, const RulePart& b) {
				return a.storeName < b.storeName; 
			}
		);
		LOG("sorted NAMES | %s\n", sorted);
		for(auto& jt : sorted) {
			format(out.astS, "%s(%s_arg), ", jt.storeName, jt.storeName);
		}
		format(out.astS, "rule(enumType)\n{\n");

		for(auto& jt : it) {
			if(rs.rules.find(jt.name) != rs.rules.end()) {
				format(out.astS, "\t%s->setParent(this);\n", jt.storeName);
			}
		}

		format(out.astS, "}\n\n");
	}

	format(out.astS, "%s::%s(", current, current);
	format(out.astS, "const %sEnum enumType)\n\t: ", current);
	format(out.astS, "rule(enumType)\n{\n}\n");

	format(out.astH, "enum class %sEnum {\n", current);
	std::set<std::string> setNames;
	for(auto& it : moreEnumNames) {
		setNames.insert(it);
	}
	const size_t enumNamesS = setNames.size();
	size_t idx = 0;
	for(auto& it : setNames) {
		format(out.astH, "\t%s%s", it, idx+1 == enumNamesS ? "\n" : ",\n");
		++idx;
	}
	format(out.astH, "};\n\n");

	format(out.astH, 
		"std::ostream& operator<<(std::ostream&, const %sEnum);\n\n", current);
	format(out.astS, 
		"\nstd::ostream& operator<<(std::ostream& ss, const %sEnum en) {\n", 
		current
	);
	bool first = true;
	for(auto& it : setNames) {
		format(out.astS, "%s(en == %sEnum::%s) {\n\t\tss<<\"%s\";\n\t}", 
			first ? "\tif" : " else if", current, it, it
		);
		first = false;
	}
	format(out.astS, 
		" else {\n\t\tthrow std::logic_error(std::string(\"No %sEnum for"
		" value \'\")\n\t\t\t + std::to_string(static_cast<int>(en)) + "
		"\"'\"\n\t\t);\n\t}\n", current
	);
	format(out.astS, "\treturn ss;\n");
	format(out.astS, "}\n\n");

	format(out.astS, "%sEnum %s::getRule() const {\n"
		"\treturn this->rule;\n"
		"}\n\n", current, current
	);

	format(out.mulS, "bool MultiVisitor::visit%s(%s* node) {\n", current, 
		current);
	format(out.mulS, "\tbool ret = true;\n");
	format(out.mulS, "\tfor(auto& it : this->visitor) {\n");
	format(out.mulS, "\t\tret &= it.get().visit%s(node);\n", current);
	format(out.mulS, "\t}\n");
	format(out.mulS, "\treturn ret;\n");
	format(out.mulS, "}\n\n");

	format(out.mulS, "bool MultiVisitor::visit%s(const %s* node) {\n", 
		current, current);
	format(out.mulS, "\tbool ret = true;\n");
	format(out.mulS, "\tfor(auto& it : this->visitor) {\n");
	format(out.mulS, "\t\tret &= it.get().visit%s(node);\n", current);
	format(out.mulS, "\t}\n");
	format(out.mulS, "\treturn ret;\n");
	format(out.mulS, "}\n\n");

	format(out.mulS, "bool MultiVisitor::leave%s(%s* node) {\n", current, 
		current);
	format(out.mulS, "\tbool ret = true;\n");
	format(out.mulS, "\tfor(auto& it : this->visitor) {\n");
	format(out.mulS, "\t\tret &= it.get().leave%s(node);\n", current);
	format(out.mulS, "\t}\n");
	format(out.mulS, "\treturn ret;\n");
	format(out.mulS, "}\n\n");

	format(out.mulS, "bool MultiVisitor::leave%s(const %s* node) {\n", 
		current, current);
	format(out.mulS, "\tbool ret = true;\n");
	format(out.mulS, "\tfor(auto& it : this->visitor) {\n");
	format(out.mulS, "\t\tret &= it.get().leave%s(node);\n", current);
	format(out.mulS, "\t}\n");
	format(out.mulS, "\treturn ret;\n");
	format(out.mulS, "}\n\n");

	format(out.dotS, "bool DotVisitor::visit%s(%s* node) {\n", current, 
		current);
	format(out.dotS, "\treturn visit%s(static_cast<const %s*>(node));\n", 
		current, current);
	format(out.dotS, "}\n\n");

	format(out.dotS, "bool DotVisitor::leave%s(%s* node) {\n", current, 
		current);
	format(out.dotS, "\treturn leave%s(static_cast<const %s*>(node));\n", 
		current, current);
	format(out.dotS, "}\n\n");
	
	format(out.dotS, "bool DotVisitor::visit%s(const %s* node) {\n", current, 
		current);
	format(out.dotS, "\tthis->writeHeader(node);\n");
	format(out.dotS, "\tss<<\"%s\"<<", current);
	format(out.dotS, "\"</font>\\n\";\n");
	format(out.dotS, "\tss<<\"\\t</td>\\n\";\n");
	format(out.dotS, "\tss<<\"</tr>\\n\";\n");
	format(out.dotS, "\tss<<\"<tr>\\n\";\n");
	format(out.dotS, "\tss<<\"\\t<td align=\\\"left\\\">Id</td>\\n\";\n");
	format(out.dotS, 
		"\tss<<\"\\t<td align=\\\"right\\\">\"<<node->getId()<<\"</td>\";\n"
	);
	format(out.dotS, "\tss<<\"</tr>\\n\";\n");
	format(out.dotS, "\tss<<\"<tr>\";\n");
	format(out.dotS, "\tss<<\"\\t<td align=\\\"left\\\">Rule</td>\\n\";\n");
	format(out.dotS, "\tss<<\"\\t<td align=\\\"right\\\">\"<<\"%s\"<<\""
		"</td>\";\n", current);
	format(out.dotS, "\tss<<\"</tr>\";\n");

	format(out.outS, "bool StdOutVisitor::visit%s(%s* node) {\n", current, 
		current);
	format(out.outS, "\treturn visit%s(static_cast<const %s*>(node));\n", 
		current, current);
	format(out.outS, "}\n\n");

	format(out.outS, "bool StdOutVisitor::leave%s(%s* node) {\n", current, 
		current);
	format(out.outS, "\treturn leave%s(static_cast<const %s*>(node));\n", 
		current, current);
	format(out.outS, "}\n\n");
	
	format(out.outS, "bool StdOutVisitor::visit%s(const %s* node) {\n", 
		current, current);
	format(out.outS, "\tmakeIndent();\n");
	format(out.outS, "\tss<<\"%s(\"<<node->getId()<<\") \";\n",
		current
	);
	format(out.outS, "\tss<<\"Rule(\"<<node->getRule()<<\")\"<<std::endl;\n");
	
	first = true;
	for(auto& it : enumNames2) {
		bool containsToken = false;
		for(auto& jt : it.second) {
			if(rs.token.count(jt.name)) {
				containsToken = true;
				goto br;
			}
		}
		br:
		if(!containsToken) {
			continue;
		}
		format(out.outS, "%s(node->getRule() == %sEnum::%s) {\n", 
			first ? "\tif" : " else if", current, it.first
		);
		format(out.dotS, "%s(node->getRule() == %sEnum::%s) {\n", 
			first ? "\tif" : " else if", current, it.first
		);
		for(auto& jt : it.second) {
			if(rs.token.count(jt.name)) {
				format(out.dotS, "\t\tss<<\"<tr%s>\";\n", first ?
					" style=\\\"border-top:2px solid #f00;\\\"" : "");
				format(out.dotS, "\t\tss<<\"\\t<td align=\\\"left\\\">"
					"Token(%s)</td>\";\n", jt.storeName);
				format(out.dotS, "\t\tss<<\"\\t<td align=\\\"right\\\">\"<<"
					"node->get%s()<<\"</td>\";\n", cap(jt.storeName)
				);
				format(out.dotS, "\t\tss<<\"</tr>\";\n");

				format(out.outS, "\t\tmakeIndent();\n");
				format(out.outS, "\t\tss<<node->get%s()<<std::endl;\n",
					cap(jt.storeName)
				);
			}
		}
		format(out.dotS, "\t}");
		format(out.outS, "\t}");
		first = false;

	}
	format(out.dotS, "\n\tss<<\"</table>>\";\n");
	format(out.dotS, "\tss<<\"];\\n\\n\";\n");
	format(out.dotS, "\treturn true;\n");
	format(out.dotS, "}\n\n");

	format(out.outS, "\n\tincrementIndent();\n");
	format(out.outS, "\treturn true;\n");
	format(out.outS, "}\n\n");

	format(out.outS, "bool StdOutVisitor::leave%s(const %s*) {\n"
		"\tdecrementIndent();\n"
		"\treturn true;\n}\n\n", current, current);
	/*first = true;
	for(auto& it : enumNames2) {
		LOG("%s", it.first);
		bool containsRules = false;
		for(auto& jt : it.second) {
			if(rs.rules.count(jt.name)) {
				containsRules = true;
				goto br4;
			}
		}
		br4:
		if(!containsRules) {
			break;
		}
		format(out.astS, "%s(this->rule == %sEnum::%s) {\n", first ? "\tif" : " else if", 
			current, it.first
		);
		LOG("%s %u", it.second, it.second.size());
		for(auto& jt : it.second) {
			if(rs.rules.count(jt.name)) {
				format(out.astS, "\t\tss<<this->%s;\n",
					jt.storeName
				);
			}
		}
		format(out.astS, "\t}");
		first = false;
	}
	format(out.astS, "\n\n");
	*/
	size_t hasCount = 0;
	first = true;
	for(auto& it : enumNames2) {
		bool containsRules = false;
		for(auto& jt : it.second) {
			if(rs.rules.count(jt.name)) {
				containsRules = true;
				goto br3;
			}
		}
		br3:
		if(!containsRules) {
			break;
		}
		for(auto& jt : it.second) {
			if(rs.rules.count(jt.name)) {
				++hasCount;
			}
		}
		first = false;
	}
	
	format(out.dotS, "bool DotVisitor::leave%s(const %s* %s) {\n", current, 
		current, hasCount ? "node" : ""
	);
	first = true;
	for(auto& it : enumNames2) {
		bool containsRules = false;
		for(auto& jt : it.second) {
			if(rs.rules.count(jt.name)) {
				containsRules = true;
				goto br2;
			}
		}
		br2:
		if(!containsRules) {
			break;
		}
		format(out.dotS, "%s(node->getRule() == %sEnum::%s) {\n", 
			first ? "\tif" : " else if", current, it.first
		);
		for(auto& jt : it.second) {
			if(rs.rules.count(jt.name)) {
				format(out.dotS, "\t\tss<<\"\\tnode\"<<node->getId()<<\" -> "
					"node\"<<node->get%s()->getId()<<\";\\n\";\n",
					cap(jt.storeName)
				);
			}
		}
		format(out.dotS, "\t}");
		first = false;
	}
	format(out.dotS, "%s\treturn true;\n}\n\n", first ? "" : "\n");
	format(out.dotS, "\n");
	
	//format(out.astS, "}\n\n");

	this->genAstClassDeclStart();
	dupCon.clear();
	for(auto& it : minimized) {
		if(it.empty()) {
			continue;
		}
		std::vector<std::string> myCon;
		for(auto& jt : it) {
			if(rs.token.find(jt.name) != rs.token.end()) {
				myCon.push_back(format("const Token&"));
			} else if(rs.rules.find(jt.name) != rs.rules.end()) {
				myCon.push_back(format("%sPtr", jt.name));
			}
		}
		LOG("myCon = %s", myCon);
		bool allreadyThere = false;
		for(auto& jDup : dupCon) {
			LOG("jDup = %s", jDup);
			if(jDup.size() != myCon.size()) {
				continue;
			}
			for(size_t i = 0; i < myCon.size(); ++i) {
				if(jDup[i] != myCon[i]) {
					goto cont2;
				}
			}
			allreadyThere = true;
			cont2: continue;
		}
		if(allreadyThere) {
			continue;
		}
		LOG("new");
		dupCon.push_back(myCon);
		format(out.astH, "\t%s(", this->current);
		for(auto& jt : it) {
			if(rs.token.find(jt.name) != rs.token.end()) {
				format(out.astH, "const Token&%s", ", ");
			} else if(rs.rules.find(jt.name) != rs.rules.end()) {
				format(out.astH, "%sPtr%s", jt.name, ", ");
			} else {
				ASSERT_T_MSG(false, format("Not reachable '%s'",jt.name));
			}
			//++idx;
		}
		format(out.astH, "const %sEnum);\n", current);
	}
	format(out.astH, "\t%s(", this->current);
	format(out.astH, "const %sEnum);\n", current);
	//this->genAstOutputMethods(enumNames2);
	this->genVisitor(enumNames2);
	this->genAstClassDeclEnd(allToStore);
}

void RecurDec::gen() {
	this->genAstForwardDecl();
	this->genRules();
	this->writeErrorStuff();
}
