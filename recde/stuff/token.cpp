#include <token.hpp>
#include <sstream>

Values::Values() {
	longLongValue = 0;
	stringValue = "";
}

bool Values::operator==(const Values& /*o*/) const {
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

Token::Token() : type(TokenType::Error_Type) {
		
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

void Token::toOutStream(std::ostream& ss, uint32_t in) const {
	std::string prefix(in, ' ');
	ss<<prefix<<(*this)<<std::endl;
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
		case TokenType::Identifier:
			ss<<std::fixed<<value.stringValue;
		case TokenType::Double_Value: 
			ss<<std::fixed<<value.doubleValue;
		case TokenType::Float_Value: 
			ss<<std::fixed<<value.floatValue;
		case TokenType::Real_Value:
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
	case TokenType::Abstract: return "ABSTRACT";
	case TokenType::Alias: return "ALIAS";
	case TokenType::Align: return "ALIGN";
	case TokenType::And: return "AND";
	case TokenType::AndAnd: return "ANDAND";
	case TokenType::Andassign: return "ANDASSIGN";
	case TokenType::Assign: return "ASSIGN";
	case TokenType::Bang: return "BANG";
	case TokenType::Bangequal: return "BANGEQUAL";
	case TokenType::Bangis: return "BANGIS";
	case TokenType::Bool: return "BOOL";
	case TokenType::Byte: return "BYTE";
	case TokenType::Byte_Value: return "BYTE_VALUE";
	case TokenType::Case: return "CASE";
	case TokenType::Cast: return "CAST";
	case TokenType::Char: return "CHAR";
	case TokenType::Char_Value: return "CHAR_VALUE";
	case TokenType::Colon: return "COLON";
	case TokenType::Comma: return "COMMA";
	case TokenType::Concat: return "CONCAT";
	case TokenType::Concatassign: return "CONCATASSIGN";
	case TokenType::Const: return "CONST";
	case TokenType::Dchar: return "DCHAR";
	case TokenType::Default: return "DEFAULT";
	case TokenType::Def: return "DEF";
	case TokenType::Div: return "DIV";
	case TokenType::Divassign: return "DIVASSIGN";
	case TokenType::Do: return "DO";
	case TokenType::Dot: return "DOT";
	case TokenType::Double: return "DOUBLE";
	case TokenType::Double_Value: return "DOUBLE_VALUE";
	case TokenType::Dstring: return "DSTRING";
	case TokenType::Dstring_Value: return "DSTRING_VALUE";
	case TokenType::Eoi: return "ENDOFINPUT";
	case TokenType::Equal: return "EQUAL";
	case TokenType::Error_Type: return "Error_Type";
	case TokenType::Float: return "FLOAT";
	case TokenType::Final: return "FINAL";
	case TokenType::Float_Value: return "FLOAT_VALUE";
	case TokenType::For: return "FOR";
	case TokenType::Foreach: return "FOREACH";
	case TokenType::Greater: return "GREATER";
	case TokenType::Greaterequal: return "GREATEREQUAL";
	case TokenType::Identifier: return "IDENTIFIER";
	case TokenType::If: return "IF";
	case TokenType::Int: return "INT";
	case TokenType::Int_Value: return "INT_VALUE";
	case TokenType::Is: return "IS";
	case TokenType::Lbrack: return "LBRACK";
	case TokenType::Lcurly: return "LCURLY";
	case TokenType::Leftshift: return "LEFTSHIFT";
	case TokenType::Leftshiftassign: return "LEFTSHIFTASSIGN";
	case TokenType::Less: return "LESS";
	case TokenType::Lessequal: return "LESSEQUAL";
	case TokenType::Logicaland: return "LOGICALAND";
	case TokenType::Logicalor: return "LOGICALOR";
	case TokenType::Logicalxor: return "LOGICALXOR";
	case TokenType::Long: return "LONG";
	case TokenType::Longlong: return "LONGLONG";
	case TokenType::Longlong_Value: return "LONGLONG_VALUE";
	case TokenType::Long_Value: return "LONG_VALUE";
	case TokenType::Lparen: return "LPAREN";
	case TokenType::Minus: return "MINUS";
	case TokenType::Minusassign: return "MINUSASSIGN";
	case TokenType::Minusminus: return "MINUSMINUS";
	case TokenType::Modulo: return "MODULO";
	case TokenType::Moduloassign: return "MODULOASSIGN";
	case TokenType::Or: return "OR";
	case TokenType::Orassign: return "ORASSIGN";
	case TokenType::Plus: return "PLUS";
	case TokenType::Plusassign: return "PLUSASSIGN";
	case TokenType::Plusplus: return "PLUSPLUS";
	case TokenType::Questionmark: return "QUESTIONMARK";
	case TokenType::Rbrack: return "RBRACK";
	case TokenType::Rcurly: return "RCURLY";
	case TokenType::Ref: return "REF";
	case TokenType::Real: return "REAL";
	case TokenType::Return: return "RETURN";
	case TokenType::Real_Value: return "REAL_VALUE";
	case TokenType::Rightshift: return "RIGHTSHIFT";
	case TokenType::Rightshiftassign: return "RIGHTSHIFTASSIGN";
	case TokenType::Rparen: return "RPAREN";
	case TokenType::Semicolon: return "SEMICOLON";
	case TokenType::Short: return "SHORT";
	case TokenType::Scope: return "SCOPE";
	case TokenType::Typeof: return "TYPEOF";
	case TokenType::Short_Value: return "SHORT_VALUE";
	case TokenType::Star: return "STAR";
	case TokenType::Starassign: return "STARASSIGN";
	case TokenType::String: return "STRING";
	case TokenType::String_Value: return "STRING_VALUE";
	case TokenType::Switch: return "SWITCH";
	case TokenType::Ubyte: return "UBYTE";
	case TokenType::Ubyte_Value: return "UBYTE_VALUE";
	case TokenType::Uint: return "UINT";
	case TokenType::Uint_Value: return "UINT_VALUE";
	case TokenType::Ulong: return "ULONG";
	case TokenType::Ulonglong: return "ULONGLONG";
	case TokenType::Ulonglong_Value: return "ULONGLONG_VALUE";
	case TokenType::Ulong_Value: return "ULONG_VALUE";
	case TokenType::Unknown: return "UNKNOWN";
	case TokenType::Urightshift: return "URIGHTSHIFT";
	case TokenType::Urightshiftassign: return "RIGHTSHIFTASSIGN";
	case TokenType::Ushort: return "USHORT";
	case TokenType::Ushort_Value: return "USHORT_VALUE";
	case TokenType::Var: return "VAR";
	case TokenType::Void: return "VOID";
	case TokenType::Wchar: return "WCHAR";
	case TokenType::While: return "WHILE";
	case TokenType::Wstring: return "WSTRING";
	case TokenType::Wstring_Value: return "WSTRING_VALUE";
	case TokenType::Xor: return "XOR";
	case TokenType::Xorassign: return "XORASSIGN";
	}
	return "UNKNOWN";
}

std::ostream& operator<<(std::ostream& o, const Token& t) {
	o<<"Token("<<t.line<<":"<<t.column<<' '<<tokenTypeToString(t.type);
	if(t.type == TokenType::Identifier) {
		o<<" value("<<t.value.stringValue<<')';
	}
	o<<')';
	return o;
}
