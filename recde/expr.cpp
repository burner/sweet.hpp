#include <expr.hpp>
#include <format.hpp>

Expr::Expr(const RuleVector& rv) : rule(rv) {
}

std::ostream& operator<<(std::ostream& s, const Expr& t) {
	for(auto& it : t.rule) {
		format(s, "%s ", it);
	}
	return s;
}
