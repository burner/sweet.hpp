#include <fstream>
#include <string>
#include <iostream>
#include <istream>
#include <sstream>
#include <memory>
#include <assert.h>

#include <lexer.hpp>
#include <logger.hpp>
#include <numberconv.hpp>
#include <unit.hpp>

Lexer::Lexer(std::shared_ptr<std::istream> in) : input(in) , column(0),
		line(0) {
	it = buffer.end();
	eof = false;
	readNext = true;
	this->getLine();
}

Lexer::Lexer(const std::string& filename) : column(0), line(0) {
	input = std::make_shared<std::ifstream>(filename.c_str(), std::ios::in);
	it = buffer.end();
	eof = false;
	readNext = true;
	this->getLine();
}

void Lexer::getLine() {
	if(!input->eof()) {
		getline(*input, buffer);
		line++;
		this->it = buffer.begin();
	} else {
		eof = true;
	}
}

char Lexer::getNextChar() {
	if(!st.empty()) {
		char t = st.top();
		st.pop();
		curInput = t;
		store<<curInput;
		return t;
	}
	if(it == buffer.end()) {
		this->getLine();
		curInput = '\n';
		store<<curInput;
		return '\n';
	} 

	if(it == buffer.end()) {
		curInput = static_cast<char>(0);
		store<<curInput;
		return curInput;
	} else {
		char tmp = *it;
		it++;
		column++;
		curInput = tmp;
		store<<curInput;
		return tmp;
	}
}

bool isParanthesis(const char input) {
	return input == '(' || input == ')';
}

bool isBracked(const char input) {
	return input == '[' || input == ']';
}

bool isCurly(const char input) {
	return input == '{' || input == '}';
}

bool isSeperator(const char input) {
	return input == ',' || input == ';' || input == ':' || input == '.';
}

bool isOperator(const char input) {
	return input == '+' || input == '-' || input == '*' || input == '/' ||
		input == '%' || input == '&' || input == '|';
}

bool isComperator(const char input) {
	return input == '?' || input == '<' || input == '>' || input == '=';
}

bool isWhiteSpace(const char input) {
	return input == ' ' || input == '\t' || input == '\n' || input == '\r';
}

bool isControlCharacter(const char input) {
	return input == '!';
}

bool isTokenStop(const char input) {
	return isParanthesis(input) || isBracked(input) || isCurly(input) ||
		isSeperator(input) || isOperator(input) || isComperator(input) ||
		isWhiteSpace(input) || isControlCharacter(input) || 
		input == static_cast<char>(0);
}

Token Lexer::parseFloat() {
	while((curInput >= '0' && curInput <= '9') || curInput == 'e' || 
			curInput == 'E' || curInput == '-' || curInput == '+' ||
			curInput == '.') {
		getNextChar();
	}
	//log("%s", store.str());

	if(curInput == 'f' || curInput == 'F') {
		std::string tmp = store.str();
		tmp.pop_back();
		return Token(TokenType::Float_Value, tokenLine, tokenColumn, 
			stringToFloat(tmp) );
	} else if(curInput == 'r' || curInput == 'R') {
		std::string tmp = store.str();
		tmp.pop_back();
		return Token(TokenType::Real_Value, tokenLine, tokenColumn,  
			stringToReal(tmp));
	} else if(curInput == 'd' || curInput == 'D') {
		std::string tmp = store.str();
		tmp.pop_back();
		return Token(TokenType::Double_Value,tokenLine, tokenColumn,  
			stringToDouble(tmp));
	} else {
		std::string tmp = store.str();
		tmp.pop_back();
		//log("%s", tmp);
		readNext = false;
		return Token(TokenType::Double_Value, tokenLine, tokenColumn,  
			stringToDouble(tmp));
	}
}

Token Lexer::nextToken() {
	tokenLine = line;
	tokenColumn = column;
	store.str("");
	while(!eof) {
		if(readNext) {
			curInput = getNextChar();
		} else {
			readNext = true;
			store<<curInput;
		}
		switch(curInput) {
		case '(':
			return Token(TokenType::Lparen, tokenLine, tokenColumn);
		case '?':
			return Token(TokenType::Questionmark, tokenLine, tokenColumn);
		case '+': 
			if(isDigit(getNextChar() && curInput != '0')) {
				goto nonDecimalBaseNumber;
			} else if(curInput == '+') {
				return Token(TokenType::Plusplus, tokenLine, tokenColumn);
			} else {
				readNext = false;
				return Token(TokenType::Plus, tokenLine, tokenColumn);
			}
		case '-':
			if(isDigit(getNextChar() && curInput != '0')) {
				goto nonDecimalBaseNumber;
			} else if(curInput == '-') {
				return Token(TokenType::Minusminus, tokenLine, tokenColumn);
			} else {
				readNext = false;
				return Token(TokenType::Minus, tokenLine, tokenColumn);
			}
		case '!':
			if(getNextChar() == '=') {
				return Token(TokenType::Bangequal, tokenLine, tokenColumn);
			} else if(curInput == 'i') { // !is
				if(getNextChar() == 's') {
					return Token(TokenType::Bangis, tokenLine, tokenColumn);
				} else {
					st.push(curInput);
					st.push('i');
					//log("%c", st.top());
					//curInput = 'i';
					//readNext = false;
					store.str("");
					return Token(TokenType::Bang, tokenLine, tokenColumn);
				}
			} else {
				readNext = false;
				return Token(TokenType::Bang, tokenLine, tokenColumn);
			}
		case '*':
			if(getNextChar() == '=') {
				return Token(TokenType::Starassign, tokenLine, tokenColumn);
			} else {
				readNext = false;
				return Token(TokenType::Star, tokenLine, tokenColumn);
			}
		case '/':
			if(getNextChar() == '=') {
				return Token(TokenType::Divassign, tokenLine, tokenColumn);
			} else {
				readNext = false;
				return Token(TokenType::Div, tokenLine, tokenColumn);
			}
		case '%':
			if(getNextChar() == '=') {
				return Token(TokenType::Moduloassign, tokenLine, tokenColumn);
			} else {
				readNext = false;
				return Token(TokenType::Modulo, tokenLine, tokenColumn);
			}
		case '.':
			return Token(TokenType::Dot, tokenLine, tokenColumn);
		case '|':
			if(getNextChar() == '=') {
				return Token(TokenType::Orassign, tokenLine, tokenColumn);
			} else if(curInput == '|') {
				return Token(TokenType::Logicalor, tokenLine, tokenColumn);
			} else {
				readNext = false;
				return Token(TokenType::Or, tokenLine, tokenColumn);
			}
		case '&':
			if(getNextChar() == '=') {
				return Token(TokenType::Andassign, tokenLine, tokenColumn);
			} else if(curInput == '&') {
				return Token(TokenType::Logicaland, tokenLine, tokenColumn);
			} else {
				readNext = false;
				return Token(TokenType::And, tokenLine, tokenColumn);
			}
		case '\'':
			if(getNextChar() == '\\') {
				if(getNextChar() == 'U') {
					assert(false); // TODO unicode character
				} else if(curInput == 'u') {
					assert(false); // TODO unicode character
				} else if(curInput == 'a') {
					return Token(TokenType::Char_Value, tokenLine,
						tokenColumn, '\a');
				} else if(curInput == 'b') {
					return Token(TokenType::Char_Value, tokenLine,
						tokenColumn, '\b');
				} else if(curInput == 't') {
					return Token(TokenType::Char_Value, tokenLine,
						tokenColumn, '\t');
				} else if(curInput == 'n') {
					return Token(TokenType::Char_Value, tokenLine,
						tokenColumn, '\n');
				} else if(curInput == 'v') {
					return Token(TokenType::Char_Value, tokenLine,
						tokenColumn, '\v');
				} else if(curInput == 'f') {
					return Token(TokenType::Char_Value, tokenLine,
						tokenColumn, '\f');
				} else if(curInput == 'r') {
					return Token(TokenType::Char_Value, tokenLine,
						tokenColumn, '\r');
				} else {
					assert(false);	//TODO undefined character
				}
			} else {
				char c = getNextChar();
				if(getNextChar() == '\'') {
					return Token(TokenType::Char_Value, tokenLine,
						tokenColumn, c);
				} else {
					assert(false);	//TODO undefined character
				}
			}
		case '^':
			if(getNextChar() == '=') {
				return Token(TokenType::Xorassign, tokenLine, tokenColumn);
			} else if(curInput == '^') {
				return Token(TokenType::Logicalxor, tokenLine, tokenColumn);
			} else {
				readNext = false;
				return Token(TokenType::Logicalor, tokenLine, tokenColumn);
			}
		case ')':
			return Token(TokenType::Rparen, tokenLine, tokenColumn);
		case '[':
			return Token(TokenType::Lbrack, tokenLine, tokenColumn);
		case ']':
			return Token(TokenType::Rbrack, tokenLine, tokenColumn);
		case '{':
			return Token(TokenType::Lcurly, tokenLine, tokenColumn);
		case '}':
			return Token(TokenType::Rcurly, tokenLine, tokenColumn);
		case ',':
			return Token(TokenType::Comma, tokenLine, tokenColumn);
		case ';':
			return Token(TokenType::Semicolon, tokenLine, tokenColumn);
		case ':':
			return Token(TokenType::Colon, tokenLine, tokenColumn);
		case '<':
			if(getNextChar() == '<') {
				if(getNextChar() == '=') {
					return Token(TokenType::Leftshiftassign, tokenLine,
						tokenColumn);
				} else {
					readNext = false;
					return Token(TokenType::Leftshift, tokenLine, tokenColumn);
				}
			} else if(curInput == '=') {
				return Token(TokenType::Lessequal, tokenLine, tokenColumn);
			} else {
				readNext = false;
				return Token(TokenType::Less, tokenLine, tokenColumn);
			}
		case '>':
			if(getNextChar() == '>') {
				if(getNextChar() == '=') {
					return Token(TokenType::Rightshiftassign, tokenLine,
						tokenColumn);
				} else if(curInput == '>') {
					if(getNextChar() == '=') {
						return Token(TokenType::Urightshiftassign, tokenLine,
								tokenColumn);
					} else {
						readNext = false;
						return Token(TokenType::Urightshift, tokenLine,
							tokenColumn);
					}
				} else {
					readNext = false;
					return Token(TokenType::Rightshift, tokenLine, tokenColumn);
				}
			} else if(curInput == '=') {
				return Token(TokenType::Greaterequal, tokenLine, tokenColumn);
			} else {
				readNext = false;
				return Token(TokenType::Greater, tokenLine, tokenColumn);
			}
		case '=':
			if(getNextChar() == '=') {
				return Token(TokenType::Equal, tokenLine, tokenColumn);
			} else {
				readNext = false;	
				return Token(TokenType::Assign, tokenLine, tokenColumn);
			}
		case '~':
			if(getNextChar() == '=') {
				return Token(TokenType::Concatassign, tokenLine, tokenColumn);
			} else {
				readNext = false;	
				return Token(TokenType::Concat, tokenLine, tokenColumn);
			}
		case 'a':
			if(getNextChar() == 'b') { // abstract
				if(getNextChar() == 's') {
					if(getNextChar() == 't') {
						if(getNextChar() == 'r') {
							if(getNextChar() == 'a') {
								if(getNextChar() == 'c') {
									if(getNextChar() == 't') {
										if(isTokenStop(getNextChar())) {
											readNext = false;
											return Token(TokenType::Abstract,
												tokenLine, tokenColumn);
										}
									}
								}
							}
						}
					}
				}
			} else if(curInput == 'l') { // alias 
				if(getNextChar() == 'i') {
					if(getNextChar() == 'a') {
						if(getNextChar() == 's') {
							if(isTokenStop(getNextChar())) {
								readNext = false;
								return Token(TokenType::Alias, tokenLine,
									tokenColumn);
							}
						}
					} else if(curInput == 'g') { // align
						if(getNextChar() == 'n') {
							if(isTokenStop(getNextChar())) {
								readNext = false;
								return Token(TokenType::Align, tokenLine,
									tokenColumn);
							}
						}
					}
				}
			}
			goto identifier;
		case 'b':
			if(getNextChar() == 'o') { // bool
				if(getNextChar() == 'o') {
					if(getNextChar() == 'l') {
						if(isTokenStop(getNextChar())) {
							readNext = false;
							return Token(TokenType::Bool, tokenLine,
								tokenColumn);
						}
					}
				}
			} else if(curInput == 'y') { // byte
				if(getNextChar() == 't') {
					if(getNextChar() == 'e') {
						if(isTokenStop(getNextChar())) {
							readNext = false;
							return Token(TokenType::Byte, tokenLine,
								tokenColumn);
						}
					}
				}
			}
			goto identifier;
		case 'c':
			if(getNextChar() == 'a') { // case
				if(getNextChar() == 's') {
					if(getNextChar() == 'e') {
						if(isTokenStop(getNextChar())) {
							readNext = false;
							return Token(TokenType::Case, tokenLine, 
								tokenColumn);
						}
					} else if(curInput == 't') {
						if(isTokenStop(getNextChar())) {
							readNext = false;
							return Token(TokenType::Cast, tokenLine, 
								tokenColumn
							);
						}
					}
				}
			} else if(curInput == 'o') {
				if(getNextChar() == 'n') {
					if(getNextChar() == 's') {
						if(getNextChar() == 't') {
							if(isTokenStop(getNextChar())) {
								readNext = false;
								return Token(TokenType::Const, tokenLine, 
									tokenColumn);
							}
						}
					}
				}
			}
			goto identifier;
		case 'd':
			if(getNextChar() == 'o') { // do
				if(isTokenStop(getNextChar())) {
					readNext = false;
					return Token(TokenType::Do, tokenLine, tokenColumn);
				}
			} else if(curInput == 'c') {
				if(getNextChar() == 'h') {
					if(getNextChar() == 'a') {
						if(getNextChar() == 'r') {
							if(isTokenStop(getNextChar())) {
								readNext = false;
								return Token(TokenType::Dchar, 
									tokenLine, tokenColumn);
							}
						}
					}
				}
			} else if(curInput == 'e') { // default
				if(getNextChar() == 'f') {
					if(getNextChar() == 'a') {
						if(getNextChar() == 'u') {
							if(getNextChar() == 'l') {
								if(getNextChar() == 't') {
									if(isTokenStop(getNextChar())) {
										readNext = false;
										return Token(TokenType::Default, 
											tokenLine, tokenColumn);
									}
								}
							}
						}
					}
				}
			}
			goto identifier;
		case 'f':
			if(getNextChar() == 'o') { // for
				if(getNextChar() == 'r') {
					if(isTokenStop(getNextChar())) {
						readNext = false;
						return Token(TokenType::For, tokenLine, tokenColumn);
					} else if(curInput == 'e') {
						if(getNextChar() == 'a') {
							if(getNextChar() == 'c') {
								if(getNextChar() == 'h') {
									if(isTokenStop(getNextChar())) {
										readNext = false;
										return Token(TokenType::Foreach, 
											tokenLine, tokenColumn
										);
									}
								}
							}
						}
					}
				}
			} else if(curInput == 'i') { // final
				if(getNextChar() == 'n') {
					if(getNextChar() == 'a') {
						if(getNextChar() == 'l') {
							if(isTokenStop(getNextChar())) {
								readNext = false;
								return Token(TokenType::Final, tokenLine, 
									tokenColumn);
							}
						}
					}
				}
			} else if(curInput == 'l') { // final
				if(getNextChar() == 'o') {
					if(getNextChar() == 'a') {
						if(getNextChar() == 't') {
							if(isTokenStop(getNextChar())) {
								readNext = false;
								return Token(TokenType::Float, tokenLine, 
									tokenColumn);
							}
						}
					}
				}
			}
			goto identifier;
		case 'i':
			if(getNextChar() == 'f') { // if
				if(isTokenStop(getNextChar())) {
					readNext = false;
					return Token(TokenType::If, tokenLine, tokenColumn);
				}
			} else if(curInput == 's') { // is
				if(isTokenStop(getNextChar())) {
					readNext = false;
					return Token(TokenType::Is, tokenLine, tokenColumn);
				}
			} else if(curInput == 'n') { // int
				if(getNextChar() == 't') { 
					if(isTokenStop(getNextChar())) {
						readNext = false;
						return Token(TokenType::Int, tokenLine, tokenColumn);
					}
				}
			}
			goto identifier;
		case 'u':
			if(getNextChar() == 's') { // ushort
				if(getNextChar() == 'h') {
					if(getNextChar() == 'o') {
						if(getNextChar() == 'r') {
							if(getNextChar() == 't') {
								if(isTokenStop(getNextChar())) {
									readNext = false;
									return Token(TokenType::Ushort, tokenLine,
										tokenColumn);
								}
							}
						}
					}
				}
			} else if(curInput == 'b') { // ubyte
				if(getNextChar() == 'y') { 
					if(getNextChar() == 't') {
						if(getNextChar() == 'e') {
							if(isTokenStop(getNextChar())) {
								readNext = false;
								return Token(TokenType::Ubyte, tokenLine,
									tokenColumn);
							}
						}
					}
				}
			} else if(curInput == 'i') { // uint
				if(getNextChar() == 'n') { 
					if(getNextChar() == 't') {
						if(isTokenStop(getNextChar())) {
							readNext = false;
							return Token(TokenType::Uint, tokenLine,
								tokenColumn);
						}
					}
				}
			} else if(curInput == 'l') { // ulong
				if(getNextChar() == 'o') { 
					if(getNextChar() == 'n') {
						if(getNextChar() == 'g') {
							if(isTokenStop(getNextChar())) {
								readNext = false;
								return Token(TokenType::Ulong, tokenLine,
									tokenColumn);
							}
						}
					}
				}
			}
			goto identifier;
		case 'r':
			if(getNextChar() == 'e') { // return
				if(getNextChar() == 't') {
					if(getNextChar() == 'u') {
						if(getNextChar() == 'r') {
							if(getNextChar() == 'n') {
								if(isTokenStop(getNextChar())) {
									readNext = false;
									return Token(TokenType::Return, 
										tokenLine, tokenColumn);
								}
							}
						}
					}
				} else if(curInput == 'f') { // ref
					if(isTokenStop(getNextChar())) {
						readNext = false;
						return Token(TokenType::Ref, 
							tokenLine, tokenColumn);
					}
				}
			}
			goto identifier;
		case 's':
			if(getNextChar() == 'w') { // switch
				if(getNextChar() == 'i') {
					if(getNextChar() == 't') {
						if(getNextChar() == 'c') {
							if(getNextChar() == 'h') {
								if(isTokenStop(getNextChar())) {
									readNext = false;
									return Token(TokenType::Switch, 
										tokenLine, tokenColumn);
								}
							}
						}
					}
				}
			} else if(curInput == 'c') { // scope
				if(getNextChar() == 'o') {
					if(getNextChar() == 'p') {
						if(getNextChar() == 'e') {
							if(isTokenStop(getNextChar())) {
								readNext = false;
								return Token(TokenType::Scope, 
									tokenLine, tokenColumn);
							}
						}
					}
				}
			}
			goto identifier;
		case 't':
			if(getNextChar() == 'y') { // typeof
				if(getNextChar() == 'p') {
					if(getNextChar() == 'e') {
						if(getNextChar() == 'o') {
							if(getNextChar() == 'f') {
								if(isTokenStop(getNextChar())) {
									readNext = false;
									return Token(TokenType::Typeof, 
										tokenLine, tokenColumn);
								}
							}
						}
					}
				}
			}
			goto identifier;
		case 'v':
			if(getNextChar() == 'o') {
				if(getNextChar() == 'i') {
					if(getNextChar() == 'd') {
						if(isTokenStop(getNextChar())) {
							readNext = false;
							return Token(TokenType::Void, 
								tokenLine, tokenColumn);
						}
					}
				}
			} else if(curInput == 'a') {
				if(getNextChar() == 'r') {
					if(isTokenStop(getNextChar())) {
						readNext = false;
						return Token(TokenType::Var, 
							tokenLine, tokenColumn);
					}
				}
			}
			goto identifier;
		case 'w':
			if(getNextChar() == 'h') { // ushort
				if(getNextChar() == 'i') {
					if(getNextChar() == 'l') {
						if(getNextChar() == 'e') {
							if(isTokenStop(getNextChar())) {
								readNext = false;
								return Token(TokenType::While, tokenLine, 
									tokenColumn);
							}
						}
					}
				}
			} else if(curInput == 'c') {
				if(getNextChar() == 'h') {
					if(getNextChar() == 'a') {
						if(getNextChar() == 'r') {
							if(isTokenStop(getNextChar())) {
								readNext = false;
								return Token(TokenType::Wchar, 
									tokenLine, tokenColumn);
							}
						}
					}
				}
			}
			goto identifier;
		case '0':
			if(getNextChar() == 'b') { // binary integer
				while(curInput == '0' || curInput == '1' || curInput == '_') {
					getNextChar();
				}
				if(curInput == 'u') {
					getNextChar();
					if(curInput == 'b') {
						return Token(TokenType::Ubyte_Value, tokenLine,
							tokenColumn, stringToUChar(store.str(), 2));
					} else if(curInput == 's') {
						return Token(TokenType::Ushort_Value, tokenLine,
							tokenColumn, stringToUShort(store.str(), 2));
					} else if(curInput == 'l') {
						return Token(TokenType::Ulong_Value, tokenLine,
							tokenColumn, stringToULong(store.str(), 2)); 
					} else if(isTokenStop(curInput)) {
						readNext = false;
						return Token(TokenType::Int_Value, tokenLine,
							tokenColumn, stringToUInt(store.str(), 2));
					}
				} else if(curInput == 'b') {
					return Token(TokenType::Byte_Value, tokenLine,
						tokenColumn, stringToChar(store.str(), 2));
				} else if(curInput == 's') {
					return Token(TokenType::Short_Value, tokenLine,
						tokenColumn, stringToShort(store.str(), 2));
				} else if(curInput == 'l') {
					return Token(TokenType::Long_Value, tokenLine, tokenColumn,
						stringToLong(store.str(), 2)); 
				} else if(isTokenStop(curInput)) {
					readNext = false;
					return Token(TokenType::Int_Value, tokenLine, tokenColumn,
						stringToInt(store.str(), 2));
				}
			} else if(curInput == 'o') { // octal integer
				getNextChar();
				while(curInput == '0' || curInput == '1' || 
						curInput == '2' || curInput == '3' || 
						curInput == '4' || curInput == '5' || 
						curInput == '6' || curInput == '7' || 
						curInput == '8' || curInput == '9' || 
						curInput == '_') {
					getNextChar();
				}
				auto tmp = store.str();
				tmp.erase(0,2);
				if(curInput == 'u') {
					getNextChar();
					if(curInput == 'b') {
						return Token(TokenType::Ubyte_Value, tokenLine,
							tokenColumn, stringToUChar(tmp, 8));
					} else if(curInput == 's') {
						return Token(TokenType::Ushort_Value, tokenLine,
							tokenColumn, stringToUShort(tmp, 8));
					} else if(curInput == 'l') {
						return Token(TokenType::Ulong_Value, tokenLine,
							tokenColumn, stringToULong(tmp, 8)); 
					} else if(isTokenStop(curInput)) {
						readNext = false;
						return Token(TokenType::Int_Value, tokenLine, 
							tokenColumn, stringToUInt(tmp, 8));
					}
				} else if(curInput == 'b') {
					return Token(TokenType::Byte_Value, tokenLine, 
						tokenColumn, stringToChar(tmp, 8));
				} else if(curInput == 's') {
					return Token(TokenType::Short_Value, tokenLine,
						tokenColumn, stringToShort(tmp, 8));
				} else if(curInput == 'l') {
					return Token(TokenType::Long_Value, tokenLine, tokenColumn,
						stringToLong(tmp, 8)); 
				} else if(isTokenStop(curInput)) {
					readNext = false;
					return Token(TokenType::Int_Value, tokenLine, tokenColumn,
						stringToInt(tmp, 8));
				}
			} else if(curInput == 'x') { // hexadezimal

			} else if(curInput >= '0' && curInput <= '9') {
				goto nonDecimalBaseNumber;
			} else {
				readNext = false;
				auto tmp = store.str();
				tmp.pop_back();
				return Token(TokenType::Int_Value, tokenLine, tokenColumn, 
					stringToInt(tmp));
			}
		case '1': case '2': case '3': case '4': case '5': case '6':
			case '7': case '8': case '9':	// decimal integer
			nonDecimalBaseNumber:
			while((curInput >= '0' && curInput <= '9') || curInput == '_') {
				getNextChar();
			}
			if(curInput == '.' || curInput == 'e' || curInput == 'E') {
				return this->parseFloat();
			} else if(curInput == 'u') {
				getNextChar();
				if(curInput == 'b') {
					return Token(TokenType::Ubyte_Value, 
						tokenLine, tokenColumn,
						stringToUChar(store.str()));
				} else if(curInput == 's') {
					return Token(TokenType::Ushort_Value, tokenLine, 
						tokenColumn, stringToUShort(store.str()));
				} else if(curInput == 'l') {
					return Token(TokenType::Ulong_Value, tokenLine, 
						tokenColumn, stringToULong(store.str())); 
				} else if(isTokenStop(curInput)) {
					readNext = false;
					return Token(TokenType::Uint_Value, tokenLine, tokenColumn,
						stringToUInt(store.str()));
				}
			} else if(curInput == 'b') {
				return Token(TokenType::Byte_Value, tokenLine, tokenColumn, 
					stringToChar(store.str()));
			} else if(curInput == 's') {
				return Token(TokenType::Short_Value, tokenLine, tokenColumn, 
					stringToShort(store.str()));
			} else if(curInput == 'l') {
				return Token(TokenType::Long_Value, tokenLine, tokenColumn,
					stringToLong(store.str())); 
			} else {
				auto tmp = store.str();
				tmp.pop_back();
				readNext = false;
				store.str("");
				return Token(TokenType::Int_Value, tokenLine, tokenColumn,
					stringToInt(tmp));
			}	
		case ' ': case '\t': case '\n': case '\r': case '\v': { // whitespace
			while(isWhiteSpace(curInput)) {
				getNextChar();
				if(eof) {
					goto EOI;
				}
			}
			store.str("");
			readNext = false;
			continue;
		}
		default:
			identifier:
			while(!isTokenStop(getNextChar())) { }
			readNext = false;
			std::string tmp = store.str();
			tmp.pop_back();	
			return Token(TokenType::Identifier, tokenLine, tokenColumn, tmp);
		}
	}
	EOI:
	return Token(TokenType::Eoi, tokenLine, tokenColumn);
}

void Lexer::print() {
	while(!eof) {
		//std::cout<<getNextChar();
		auto nt = nextToken();
		std::cout<<nt.toString()<<" ";
		if(nt.type == TokenType::Eoi) {
			break;
		}
	}
	std::cout<<std::endl;
}

/*UNITTEST(isParanthesisTest) {
	AS_T(isParanthesis('('));
	AS_T(isParanthesis(')'));
	AS_T(!isParanthesis(']'));
}
UNITTEST(isBracked) {
	AS_T(isBracked('['));
	AS_T(isBracked(']'));
	AS_T(!isBracked('('));
}
UNITTEST(isCurlyTest) {
	AS_T(isCurly('{'));
	AS_T(isCurly('}'));
	AS_T(!isCurly('['));
}
UNITTEST(isSeperatorTest) {
	AS_T(isSeperator(','));
	AS_T(isSeperator(';'));
	AS_T(isSeperator(':'));
	AS_T(isSeperator('.'));
	AS_T(!isSeperator('['));
}
UNITTEST(isOperatorTest) {
	AS_T(isOperator('+'));
	AS_T(isOperator('-'));
	AS_T(isOperator('*'));
	AS_T(isOperator('/'));
	AS_T(isOperator('%'));
	AS_T(isOperator('&'));
	AS_T(isOperator('|'));
	AS_T(!isOperator('['));
}
UNITTEST(isComperatorTest) {
	AS_T(isComperator('<'));
	AS_T(isComperator('>'));
	AS_T(isComperator('?'));
	AS_T(isComperator('='));
	AS_T(!isComperator('['));
}
UNITTEST(isWhiteSpaceTest) {
	AS_T(isWhiteSpace(' '));
	AS_T(isWhiteSpace('	'));
	AS_T(isWhiteSpace('\n'));
	AS_T(isWhiteSpace('\r'));
	AS_T(!isWhiteSpace('['));
}
UNITTEST(isControlCharacterTest) {
	AS_T(isControlCharacter('!'));
	AS_T(!isControlCharacter('['));
}*/


