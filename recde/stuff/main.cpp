#include <parser.hpp>
#include <logger.hpp>
#include <unit.hpp>

UNITTEST(unary1) {
	auto ss = std::make_shared<std::stringstream>
		("hello");
	Lexer l(ss);
	Parser p(l);
	auto ast = p.parseMulExpression();
	ast->toOutStream(std::cout);
	std::cout<<std::endl;
}

UNITTEST(rel1) {
	auto ss = std::make_shared<std::stringstream>
		("hello <= (foobar)");
	Lexer l(ss);
	Parser p(l);
	auto ast = p.parseOrOrExpression();
	ast->toOutStream(std::cout);
	std::cout<<std::endl;
}

UNITTEST(assign1) {
	auto ss = std::make_shared<std::stringstream>
		("hello = (foobar)");
	Lexer l(ss);
	Parser p(l);
	auto ast = p.parseAssignmentExpression();
	ast->toOutStream(std::cout);
	std::cout<<std::endl;
}

UNITTEST(varDecl1) {
	auto ss = std::make_shared<std::stringstream>
		("var foo(10) ");
	Lexer l(ss);
	Parser p(l);
	auto ast = p.parseVarDecl();
	ast->toOutStream(std::cout);
	std::cout<<std::endl;
}

UNITTEST(varDecl2) {
	auto ss = std::make_shared<std::stringstream>
		("var bar : int");
	Lexer l(ss);
	Parser p(l);
	auto ast = p.parseVarDecl();
	ast->toOutStream(std::cout);
	std::cout<<std::endl;
}

int main() {
	Unit::runTests();
	return 0;
}
