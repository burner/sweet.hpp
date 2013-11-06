#pragma once

#include <token.hpp>
#include <sstream>
#include <stack>
#include <memory>

class Lexer {
	private:
		std::shared_ptr<std::istream> input;
		std::string buffer;	
		std::string::iterator it;
		std::stringstream store;
		unsigned column;
		unsigned line;
		bool eof;
		bool readNext;
		char curInput;
		std::stack<char> st;

		unsigned tokenLine, tokenColumn;

		void getLine();
		char getNextChar();

		Token parseFloat();
	
	public:
		Lexer(std::shared_ptr<std::istream> in);
		Lexer(const std::string& filename);

		Token nextToken();
		void print();
};
