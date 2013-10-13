#include <token.hpp>

Token::Token(const std::string& n, const std::string& r, const std::string& c) :
		name(n), regex(r), convertFunction(c) {
}

std::ostream& operator<<(std::ostream& s, const Token& t) {
	format(s, "Token(name('%s'), regex('%s'), convertFunction('%s'))", t.name, t.regex, t.convertFunction);
	return s;
}
