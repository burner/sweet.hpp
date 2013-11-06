#include <token.hpp>
#include <sstream>

Values::Values() {
	longLongValue = 0;
	stringValue = "";
}

bool Values::operator==(const Values& o) const {
	//return longLongValue == o.longLongValue && stringValue ==
		//o.stringValue;
	return true;
}

Values::Values(const std::string& txt) : stringValue(txt) { 
}

Values::Values(const char v) { 
	charValue = v;
}

Values::Values(const short v) {
	shortValue = v;
}

Values::Values(const int v) {
	intValue = v;
}
 
Values::Values(const long v) { 
	longValue = v;
}

Values::Values(const long long v) { 
	longLongValue = v;
}

Values::Values(const unsigned char v) { 
	uCharValue = v;
}

Values::Values(const unsigned short v) {
	uShortValue = v;
}

Values::Values(const unsigned int v) { 
	uIntValue = v;
}

Values::Values(const unsigned long v) { 
	uLongValue = v;
}

Values::Values(const unsigned long long v) { 
	uLongLongValue = v;
}


Values::Values(const float v) { 
	floatValue = v;
}

Values::Values(const double v) { 
	doubleValue = v;
}

Values::Values(const long double v) { 
	longDoubleValue = v;
}

Token::Token() : type(TokenType::ERROR_TYPE) {
		
}

/*Token::Token(const Token& o) : type(o.type), line(o.line), column(c.column),
		value.stringValue(o.value.stringValue) {

}*/

Token::Token(const TokenType t, unsigned l, unsigned c) : type(t), line(l), column(c) {
}

Token::Token(const TokenType t, unsigned l, unsigned c, 
		const std::string& txt) : type(t), line(l), column(c) {
	value.stringValue = txt;
}

Token::Token(const TokenType t, unsigned l, unsigned c, const char v) :
		type(t), line(l), column(c) { 
	value.charValue = v;
}

Token::Token(const TokenType t, unsigned l, unsigned c, const short v) :
		type(t), line(l), column(c) {
	value.shortValue = v;
}

Token::Token(const TokenType t, unsigned l, unsigned c, const int v) :
		type(t), line(l), column(c) {
	value.intValue = v;
}
 
Token::Token(const TokenType t, unsigned l, unsigned c, const long v) :
		type(t), line(l), column(c) { 
	value.longValue = v;
}

Token::Token(const TokenType t, unsigned l, unsigned c, const unsigned char v) :
		type(t), line(l), column(c) { 
	value.uCharValue = v;
}

Token::Token(const TokenType t, unsigned l, unsigned c, 
		const unsigned short v) : type(t), line(l), column(c) { 
	value.uShortValue = v;
}

Token::Token(const TokenType t, unsigned l, unsigned c, const unsigned int v) : 
		type(t), line(l), column(c) { 
	value.uIntValue = v;
}

Token::Token(const TokenType t, unsigned l, unsigned c, const unsigned long v) :
		 type(t), line(l), column(c) { 
	value.uLongValue = v;
}

Token::Token(const TokenType t, unsigned l, unsigned c, const float v) :
		type(t), line(l), column(c) { 
	value.floatValue = v;
}

Token::Token(const TokenType t, unsigned l, unsigned c, const double v) : 
		type(t), line(l), column(c) { 
	value.doubleValue = v;
}

Token::Token(const TokenType t, unsigned l, unsigned c, const long double v) : 
		type(t), line(l), column(c) { 
	value.longDoubleValue = v;
}

Token::~Token() { }

/*Token& Token::operator=(const Token& o) {
	this->type = o.type;
	this->line = o.line;
	this->column = o.column;
	this->value.stringValue = o.value.stringValue;
	return this;
}*/

Loc Token::getLocation() const {
	return Loc(line, column);
}

std::string Token::toString() const {
	std::string t;
	std::stringstream ss;
	ss<<line<<":"<<column<<": ";
	switch(type) {
		case TokenType::IDENTIFIER:
			ss<<std::fixed<<value.stringValue;
		case TokenType::DOUBLE_VALUE: 
			ss<<std::fixed<<value.doubleValue;
		case TokenType::FLOAT_VALUE: 
			ss<<std::fixed<<value.floatValue;
		case TokenType::REAL_VALUE:
			ss<<std::fixed<<value.longDoubleValue;
			t = ss.str();
			break;
		default:
			t = "";
			break;
	}
	return tokenTypeToString(type) + "("+ t + ")";
}

std::string tokenTypeToString(const TokenType tt) {
	switch(tt) {
	case TokenType::ABSTRACT: return "ABSTRACT";
	case TokenType::ALIAS: return "ALIAS";
	case TokenType::ALIGN: return "ALIGN";
	case TokenType::AND: return "AND";
	case TokenType::ANDASSIGN: return "ANDASSIGN";
	case TokenType::ASSIGN: return "ASSIGN";
	case TokenType::BANG: return "BANG";
	case TokenType::BANGEQUAL: return "BANGEQUAL";
	case TokenType::BANGIS: return "BANGIS";
	case TokenType::BOOL: return "BOOL";
	case TokenType::BYTE: return "BYTE";
	case TokenType::BYTE_VALUE: return "BYTE_VALUE";
	case TokenType::CASE: return "CASE";
	case TokenType::CHAR: return "CHAR";
	case TokenType::CHAR_VALUE: return "CHAR_VALUE";
	case TokenType::COLON: return "COLON";
	case TokenType::COMMA: return "COMMA";
	case TokenType::CONCAT: return "CONCAT";
	case TokenType::CONCATASSIGN: return "CONCATASSIGN";
	case TokenType::CONST: return "CONST";
	case TokenType::DCHAR: return "DCHAR";
	case TokenType::DEFAULT: return "DEFAULT";
	case TokenType::DIV: return "DIV";
	case TokenType::DIVASSIGN: return "DIVASSIGN";
	case TokenType::DO: return "DO";
	case TokenType::DOT: return "DOT";
	case TokenType::DOUBLE: return "DOUBLE";
	case TokenType::DOUBLE_VALUE: return "DOUBLE_VALUE";
	case TokenType::DSTRING: return "DSTRING";
	case TokenType::DSTRING_VALUE: return "DSTRING_VALUE";
	case TokenType::EOI: return "ENDOFINPUT";
	case TokenType::EQUAL: return "EQUAL";
	case TokenType::ERROR_TYPE: return "ERROR_TYPE";
	case TokenType::FLOAT: return "FLOAT";
	case TokenType::FINAL: return "FINAL";
	case TokenType::FLOAT_VALUE: return "FLOAT_VALUE";
	case TokenType::FOR: return "FOR";
	case TokenType::GREATER: return "GREATER";
	case TokenType::GREATEREQUAL: return "GREATEREQUAL";
	case TokenType::IDENTIFIER: return "IDENTIFIER";
	case TokenType::IF: return "IF";
	case TokenType::INT: return "INT";
	case TokenType::INT_VALUE: return "INT_VALUE";
	case TokenType::IS: return "IS";
	case TokenType::LBRACK: return "LBRACK";
	case TokenType::LCURLY: return "LCURLY";
	case TokenType::LEFTSHIFT: return "LEFTSHIFT";
	case TokenType::LEFTSHIFTASSIGN: return "LEFTSHIFTASSIGN";
	case TokenType::LESS: return "LESS";
	case TokenType::LESSEQUAL: return "LESSEQUAL";
	case TokenType::LOGICALAND: return "LOGICALAND";
	case TokenType::LOGICALOR: return "LOGICALOR";
	case TokenType::LOGICALXOR: return "LOGICALXOR";
	case TokenType::LONG: return "LONG";
	case TokenType::LONGLONG: return "LONGLONG";
	case TokenType::LONGLONG_VALUE: return "LONGLONG_VALUE";
	case TokenType::LONG_VALUE: return "LONG_VALUE";
	case TokenType::LPAREN: return "LPAREN";
	case TokenType::MINUS: return "MINUS";
	case TokenType::MINUSASSIGN: return "MINUSASSIGN";
	case TokenType::MINUSMINUS: return "MINUSMINUS";
	case TokenType::MODULO: return "MODULO";
	case TokenType::MODULOASSIGN: return "MODULOASSIGN";
	case TokenType::OR: return "OR";
	case TokenType::ORASSIGN: return "ORASSIGN";
	case TokenType::PLUS: return "PLUS";
	case TokenType::PLUSASSIGN: return "PLUSASSIGN";
	case TokenType::PLUSPLUS: return "PLUSPLUS";
	case TokenType::QUESTIONMARK: return "QUESTIONMARK";
	case TokenType::RBRACK: return "RBRACK";
	case TokenType::RCURLY: return "RCURLY";
	case TokenType::REF: return "REF";
	case TokenType::REAL: return "REAL";
	case TokenType::RETURN: return "RETURN";
	case TokenType::REAL_VALUE: return "REAL_VALUE";
	case TokenType::RIGHTSHIFT: return "RIGHTSHIFT";
	case TokenType::RIGHTSHIFTASSIGN: return "RIGHTSHIFTASSIGN";
	case TokenType::RPAREN: return "RPAREN";
	case TokenType::SEMICOLON: return "SEMICOLON";
	case TokenType::SHORT: return "SHORT";
	case TokenType::SCOPE: return "SCOPE";
	case TokenType::TYPEOF: return "TYPEOF";
	case TokenType::SHORT_VALUE: return "SHORT_VALUE";
	case TokenType::STAR: return "STAR";
	case TokenType::STARASSIGN: return "STARASSIGN";
	case TokenType::STRING: return "STRING";
	case TokenType::STRING_VALUE: return "STRING_VALUE";
	case TokenType::SWITCH: return "SWITCH";
	case TokenType::UBYTE: return "UBYTE";
	case TokenType::UBYTE_VALUE: return "UBYTE_VALUE";
	case TokenType::UINT: return "UINT";
	case TokenType::UINT_VALUE: return "UINT_VALUE";
	case TokenType::ULONG: return "ULONG";
	case TokenType::ULONGLONG: return "ULONGLONG";
	case TokenType::ULONGLONG_VALUE: return "ULONGLONG_VALUE";
	case TokenType::ULONG_VALUE: return "ULONG_VALUE";
	case TokenType::UNKNOWN: return "UNKNOWN";
	case TokenType::URIGHTSHIFT: return "URIGHTSHIFT";
	case TokenType::URIGHTSHIFTASSIGN: return "RIGHTSHIFTASSIGN";
	case TokenType::USHORT: return "USHORT";
	case TokenType::USHORT_VALUE: return "USHORT_VALUE";
	case TokenType::VAR: return "VAR";
	case TokenType::VOID: return "VOID";
	case TokenType::WCHAR: return "WCHAR";
	case TokenType::WHILE: return "WHILE";
	case TokenType::WSTRING: return "WSTRING";
	case TokenType::WSTRING_VALUE: return "WSTRING_VALUE";
	case TokenType::XOR: return "XOR";
	case TokenType::XORASSIGN: return "XORASSIGN";
	}
	return "UNKNOWN";
}

std::ostream& operator<<(std::ostream& o, const Token& t) {
	o<<'('<<t.line<<":"<<t.column<<' '<<tokenTypeToString(t.type);
	if(t.type == TokenType::IDENTIFIER) {
		o<<" value("<<t.value.stringValue<<')';
	}
	o<<')';
	return o;
}
