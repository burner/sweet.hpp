#include <parser.hpp>
#include <logger.hpp>
#include <unit.hpp>

UNITTEST(unary1) {
	auto ss = std::make_shared<std::stringstream>
		("hello");
	Lexer l(ss);
	Parser p(l);
	auto ast = p.parseStart();
	ast->toOutStream(std::cout);
	std::cout<<std::endl;
}

UNITTEST(rel1) {
	auto ss = std::make_shared<std::stringstream>
		("hello <= (foobar)");
	Lexer l(ss);
	Parser p(l);
	auto ast = p.parseStart();
	ast->toOutStream(std::cout);
	std::cout<<std::endl;
}

int main() {
	Unit::runTests();
	return 0;
}
