#include "amber.hpp"
#include <iostream>
#include <fstream>
#include <istream>
#include <utility>
#include <cctype>
#include <stdexcept>

Pos::Pos() : row(1), column(1) {}

AstBase::AstBase(const Pos& p) : pos(p) {}

Node::Node(std::string&& t, std::string&& cl, std::string&& i, 
	std::string&& a, Children&& c, const Pos& p) : AstBase(p), openClose(false),
	classLit(std::move(cl)), idLit(std::move(i)), attributes(std::move(a)),
   	type(std::move(t)), children(std::move(c)) {
}

Node::Node(std::string&& t, std::string&& cl, std::string&& i, 
	std::string&& a, const Pos& p) : AstBase(p), openClose(true), 
	classLit(std::move(cl)), idLit(std::move(i)), attributes(std::move(a)),
   	type(std::move(t)) {
}

CNode::CNode(std::string&& p, const Pos& po) : AstBase(po), 
	program(std::move(p)) 
{
}

TNode::TNode(std::string&& l, const Pos& po) : AstBase(po), 
	line(std::move(l)) 
{
}

template<typename I>
bool increment(I& iter, Pos& pos, const size_t cnt = 1) {
	bool ret = false;
	if(*iter == '\n') {
		++pos.row;
		pos.column = 1u;
		ret = true;
	} else {
		++pos.column;
	}

	iter += cnt;
	return ret;
}

template<typename I>
void eatWhitespace(I& be, I& en, Pos& pos) {
	while(be != en) {
		const auto b = *be;
		if(b != ' ' && b != '\t' && b != '\n') {
			break;
		}
		if(b == '\n') {
			pos.row++;
			pos.column = 1;
		} else {
			pos.column++;
		}

		++be;
	}
}

template<typename I>
void eat(I& iter, const char toEat, Pos& pos) {
	if(*iter == toEat) {
		increment(iter, pos);	
	} else {
		throw std::logic_error(std::string("Expected ") + toEat);
	}
}

template<typename I>
void eat(I& iter, const std::string& toEat, Pos& pos) {
	const auto toEatS = toEat.size();
	for(size_t pos = 0; pos < toEatS; ++pos) {
		if(*(iter+pos) != toEat[pos]) {	
			throw std::logic_error(std::string("Expected ") + toEat);
		}
	}

	increment(iter, pos, toEatS);
}

template<typename I>
I eatUntil(I& be, I& en, const std::string& until, Pos& pos) {
	auto it = be;
	//for(; it != en && *it != until; increment(it, pos)) {
	//}

	while(it != en) {
		for(auto c : until) {
			if(c == *it) {
				goto found;
			}
		}

		increment(it, pos);
	}

	found:

	return it;
}

template<typename I>
NPtr parseC(I& be, I& en, Pos& pos) {
	eat(be, "<{{", pos);

	auto beCopy = be;

	for(; beCopy != en; increment(beCopy, pos)) {
		if(test(beCopy, en, "}}>")) {
			auto code = std::string(be, beCopy);
			be = beCopy;
			eat(be, "}}>", pos);
			return std::make_unique<CNode>(std::move(code), pos);	
		}
	}

	throw std::logic_error("Input ended before C statement finished");
}

template<typename I>
NPtr parseNode(I& be, I& en, Pos& pos) {
	eat(be, '<', pos);
	eatWhitespace(be, en, pos);

	auto beCopy = be;
	for(; beCopy != en && std::isalnum(*beCopy); increment(beCopy, pos)) {}

	auto type = std::string(be, beCopy);

	be = beCopy;

	std::string classLit; // .
	std::string idLit;    // #
	std::string attributes; // ( **** )

	while(*be == '.' || *be == '#') {
		beCopy = be;
		increment(beCopy, pos); // eat the . or '#'
		while(beCopy != en && (std::isalnum(*beCopy) || *beCopy == '-')) {
			increment(beCopy, pos);
		}

		if(*be == '.') {
			classLit.insert(classLit.begin(), be+1, beCopy);
		} else if(*be == '#') {
			idLit.insert(idLit.begin(), be+1, beCopy);
		}

		be = beCopy;
	}

	if(*be == '(') {
		beCopy = be + 1;
		for(; *beCopy != ')'; increment(beCopy, pos)) {}

		attributes.insert(attributes.begin(), be+1, beCopy);

		be = beCopy;
		eat(be, ')', pos);
	}

	eatWhitespace(be, en, pos);

	if(test(be, en, "/>")) {
		eat(be, "/>", pos);
		return std::move(std::make_unique<Node>(std::move(type), 
			std::move(classLit), std::move(idLit), std::move(attributes),
			pos));
	} else {
		auto children = mainParse(be, en, pos);
	
		return std::move(std::make_unique<Node>(std::move(type), 
			std::move(classLit), std::move(idLit), std::move(attributes),
			std::move(children), pos));
	}
}

template<typename I>
Children mainParse(I& be, I& en,  Pos& pos) {
	Children ret;

	while(be != en) {
		eatWhitespace(be, en, pos);

		if(test(be, en, "<{{")) {
			ret.push_back(std::move(parseC(be, en, pos)));	
		} else if(test(be, en, '<')) {
			ret.push_back(std::move(parseNode(be, en, pos)));
		} else if(test(be, en, '>')) {
			eat(be, '>', pos);
			break;
		} else {
			//std::cout<<*be<<std::endl;
			I iter = eatUntil(be, en, "\n>", pos);
			if(!test(be,iter,"&{{") && *be == '&') {
				++iter;
			}
			auto posCopy = pos;
			ret.push_back(std::move(
				std::make_unique<TNode>(std::move(
					std::string(be, iter)), posCopy
				)
			));

			be = iter;

			char iterC = *iter;
			eat(be, iterC, pos);

			//if(dynamic_cast<TNode*>(ret.back().get())->line.front() != '&'
			//&&
			if(iterC == '>') 
			{
				break;
			}
		}
		eatWhitespace(be, en, pos);
	}

	return ret;
}

int main(int argc, char **argv) {
	char const* filename;
	if(argc > 1) {
		filename = argv[1];
	} else {
		std::cerr << "Error: No input file provided." << std::endl;
		return 1;
	}

	std::ifstream in(filename, std::ios_base::in);

	if(!in) {
		std::cerr << "Error: Could not open input file: "
			<< filename << std::endl;
		return 1;
	}

	std::string storage; // We will read the contents here.
	in.unsetf(std::ios::skipws); // No white space skipping!
	std::copy(
		std::istream_iterator<char>(in),
		std::istream_iterator<char>(),
		std::back_inserter(storage));

	Pos pos;
	auto b = storage.begin();
	auto e = storage.end();
	auto rslt = mainParse(b, e, pos);

	for(auto& it : rslt) {
		it->gen(std::cout, 0);
	}

	return 0;
}
