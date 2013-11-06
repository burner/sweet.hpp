#pragma once

#include <string>
#include <location.hpp>

struct Values {
	Values();
	explicit Values(const std::string&);

	explicit Values(const char); 
	explicit Values(const short);
	explicit Values(const int);
	explicit Values(const long); 
	explicit Values(const long long); 
	explicit Values(const unsigned char); 
	explicit Values(const unsigned short);
	explicit Values(const unsigned int); 
	explicit Values(const unsigned long); 
	explicit Values(const unsigned long long); 
	explicit Values(const float); 
	explicit Values(const double); 
	explicit Values(const long double); 
	union {
	char charValue; unsigned char uCharValue;
	short shortValue; unsigned short uShortValue;
	int intValue; unsigned int uIntValue; 
	long longValue; unsigned long uLongValue; 
	long long longLongValue; unsigned long uLongLongValue; 
	float floatValue; double doubleValue; long double longDoubleValue; 
	};
	std::string stringValue;
	bool operator==(const Values&) const;
};

enum class TokenType {
	Abstract, // done			"abstract"
	Alias, // done				"alias"
	Align, // done				"align"
	And, // done				"&&"
	Andassign, // done			"&="
	Assign, // done				"="
	Bang, // done				"!"
	Bangequal, // done			"!="
	Bangis, // done				"!is"
	Bool, // done				"bool"
	Byte, // done				"byte"
	Byte_Value,//				"0b[01]+b|0x[0-9af]+b|0o[0-8]b"
	Case,		//				"case"
	Char,		//				"char"
	Char_Value,	//				"'[ansi]'"
	Colon, // done				":"
	Const, // done				"const"
	Comma, // done				","
	Concat, // done				"~"
	Concatassign, // done		"~="
	Default,	//				"default"
	Div, // done				"/"
	Divassign, // done			"/="
	Do, // done					"do"
	Dot, // done				"."
	Double,	//					"double"
	Double_Value,//				"[0-9]+.[0-9]+"
	Dstring,	//				"dstring"
	Dstring_Value,//			"\"[utf]*\"d"
	Eoi, // done
	Equal, // done				"=="
	Error_type,
	Float,	//					"float"
	Final,	//					"final"
	Float_Value, //				"[0-9]+.[0-9]+"
	For,	//					"for"
	Greater, // done			">"
	Greaterequal, // done 		">="
	Identifier,	//				"[a-zA-Z_]+"
	If,	//						"if"
	Int,	//					"int"
	Int_Value,//				"[0-9]+"
	Is,	//						"is"
	Lbrack, // done				"["
	Lcurly, // done				"{"
	Leftshift, // done			"<<"
	Leftshiftassign, // done 	"<<="
	Less, // done				"<"
	Lessequal, // done			"<="
	Logicaland,	//				"&&"
	Logicalor, // done			"||"
	Logicalxor, // done 		"^^"
	Long,	//					"long"
	Longlong,	//				"longlong"
	Longlong_Value, //			"[0-9]+ll"
	Long_Value,	//				"[0-9]+l"
	Lparen, // done				")"
	Minus, // done				"-"
	Minusassign, // done 		"-="
	Minusminus, // done			"--"
	Modulo, // done				"%"
	Moduloassign, // done		"%="
	Or, // done					"|"
	Orassign, // done			"|="
	Plus, // done				"+"
	Plusassign, // done			"+="
	Plusplus, // done			"++"
	Questionmark, //			"?"
	Rbrack, // done				"]"
	Rcurly, // done				"}"
	Real,//						"real"
	Ref,//						"ref"
	Return,//					"return"
	Real_Value,	//				"[0-9]+.[0-9]+r"
	Rightshift, // done			">>"
	Rightshiftassign, // done	">>="
	Rparen, // done				")"
	Semicolon, // done			";"
	Short,//					"short"
	Short_Value,//				"[0-9]+s"
	Star, // done				"*"
	Starassign,	//				"*="
	Typeof,	//					"typeof"
	String,	//					"string"
	Scope,	//					"scope"
	String_Value,//				"[utf]*"
	Switch,		//				"switch"
	Ubyte,	//					"ubyte"
	Ubyte_Value,//				"[0-9]+ub"
	Uint,//						"uint"
	Uint_Value,//				"[0-9]+u"
	Ulong,//					"ulong"
	Ulonglong,//				"ulonglong"
	Ulonglong_Value,//			"[0-9]+ull"
	Ulong_Value,//				"[0-9]ul"
	Unknown,//
	Urightshift, // done		">>>"
	Urightshiftassign, // done	">>>="
	Ushort,//					"ushort"
	Ushort_Value,//				"[0-9]+us"
	While, // done				"while"
	Wchar, // done				"wchar"
	Dchar, // done				"dchar"
	Wstring,//					"wstring"
	Wstring_Value,	//			"\"[utf]*\"w"
	Xor,	//					"^"
	Var,	//					"var"
	Void,	//					"void"
	Xorassign, // done			"^="
};

class Token {
	public:
	TokenType type;

	/// position of token in source
	unsigned line, column;

	Values value;

	Token();
	Token(const TokenType t, unsigned l, unsigned c);
	Token(const TokenType t, unsigned l, unsigned c, const std::string& txt);
	Token(const TokenType t, unsigned l, unsigned c, const char v);
	Token(const TokenType t, unsigned l, unsigned c, const short v);
	Token(const TokenType t, unsigned l, unsigned c, const int v);
	Token(const TokenType t, unsigned l, unsigned c, const long v);
	Token(const TokenType t, unsigned l, unsigned c, const unsigned char v);
	Token(const TokenType t, unsigned l, unsigned c, const unsigned short v);
	Token(const TokenType t, unsigned l, unsigned c, const unsigned int v);
	Token(const TokenType t, unsigned l, unsigned c, const unsigned long int v);
	Token(const TokenType t, unsigned l, unsigned c, const float v);
	Token(const TokenType t, unsigned l, unsigned c, const double v);
	Token(const TokenType t, unsigned l, unsigned c, const long double v);
	~Token();

	std::string toString() const;
	void toOutStream(std::ostream&, uint32_t) const;
	Loc getLocation() const;

};

std::ostream& operator<<(std::ostream&, const Token&);
std::string tokenTypeToString(const TokenType tt);
