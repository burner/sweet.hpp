// Copyright: Robert "burner" Schadek rburners@gmail.com License: GPL 3 or higher
#include "amber.hpp"
#include <iostream>
#include <fstream>
#include <istream>
#include <utility>
#include <cctype>
#include <stdexcept>
#include <options.hpp>

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

TNode::TNode(std::string&& l, const Pos& po, const TNodeType t) : AstBase(po), 
	line(std::move(l)), type(t)
{
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
NPtr parseHeader(I& be, I& en, Pos& pos) {
	eatWhitespaceComment(be, en, pos);

	auto beCopy = be;
	if(test(beCopy, en, "<{{")) {
		return parseC(be, en, pos);
	} else {
		return std::make_unique<CNode>("", pos);
	}
}

template<typename I>
NPtr parseNode(I& be, I& en, Pos& pos) {
	eat(be, '<', pos);
	eatWhitespaceComment(be, en, pos);

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
		assert(beCopy != en);
		bool tickEnclosed = false;
		if(*beCopy == '"') {
			tickEnclosed = true;
			increment(beCopy, pos); // eat the "
			while(beCopy != en && *beCopy != '"') {
				increment(beCopy, pos);
			}
			assert(*beCopy == '"');
			increment(beCopy, pos); // eat the "
		} else {
			while(beCopy != en && (std::isalnum(*beCopy) || *beCopy == '-')) {
				increment(beCopy, pos);
			}
		}

		if(*be == '.') {
			classLit.insert(classLit.begin(), tickEnclosed ? be+2 : be+1, 
				tickEnclosed ? beCopy-1 : beCopy);
		} else if(*be == '#') {
			idLit.insert(idLit.begin(), tickEnclosed ? be+2 : be+1, 
				tickEnclosed ? beCopy-1 : beCopy);
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

	eatWhitespaceComment(be, en, pos);

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
		eatWhitespaceComment(be, en, pos);

		if(test(be, en, "<{{")) {
			ret.push_back(std::move(parseC(be, en, pos)));	
		} else if(test(be, en, '<')) {
			ret.push_back(std::move(parseNode(be, en, pos)));
		} else if(test(be, en, '>')) {
			eat(be, '>', pos);
			break;
		} else {
			I iter;
			TNodeType type;	
			if(!test(be, en, "&{{") && *be == '&') {
		   		iter = eatUntil(be, en, "\n", pos);
				/*for(iter = be; !test(iter, en, '\n') && !test(iter, en, "//"); 
						increment(iter, pos)) 
				{
				}
				if(test(be, en, "//")) {
					eatWhitespaceComment(be, en, pos);
				}*/
				type = TNodeType::SingleCppLine;
			} else {
		   		iter = eatUntil(be, en, "\n>", pos);
				for(iter = be; 
					!test(iter, en, '\n') && !test(iter, en, "//") && !test(iter, en, '>'); 
					increment(iter, pos)) 
				{
				}
				if(test(be, en, "//")) {
					eatWhitespaceComment(be, en, pos);
				}
				type = TNodeType::Text;
			}
			auto posCopy = pos;
			//std::cout<<"||| "<<std::string(be, iter)<<std::endl;
			ret.push_back(std::move(
				std::make_unique<TNode>(std::move(
					std::string(be, iter)), posCopy, type
				)
			));

			be = iter;

			char iterC = *iter;
			eat(be, iterC, pos);

			if(iterC == '>') 
			{
				break;
			}
		}
		eatWhitespaceComment(be, en, pos);
	}

	return ret;
}

static const CmdOptions parseOptions(int argc, char** argv) {
	CmdOptions ret;

	sweet::Options opt(argc, argv);
	opt.get("-i", "--input", "The input file", ret.input);
	opt.get("-o", "--output", "The output file", ret.output);
	opt.get("-d", "--doctype", "The DOCTYPE of the produced file. By default it is html", ret.output);
	opt.get("-c", "--nodocttype", "If set no DOCTYPE will be written.", ret.noDocType);
	opt.get("-n", "--function", "The function name", ret.functionName);
	opt.finalize();

	ret.allOk = true;

	return ret;
}

int main(int argc, char** argv) {
	const CmdOptions opt = parseOptions(argc, argv);

	if(!opt.allOk) {
		return 1;
	}

	std::ifstream in(opt.input, std::ios_base::in);

	if(!in) {
		std::cerr << "Error: Could not open input file: \'"
			<< opt.input << "\'" << std::endl;
		return 1;
	}

	std::ostream* output = &std::cout;

	std::ofstream oFile;

	if(!opt.output.empty()) {
		oFile.open(opt.output, std::ofstream::out);	

		output = &oFile;
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

	*output<<"//This file is autogenerated from file: "<<opt.input<<"\n";
	*output<<"#pragma once\n";
	*output<<"\n#include <ostream>\n";

	auto header = parseHeader(b, e, pos);
	header->gen(*output, 0);

	*output<<"template<typename O, typename P>\n";
	*output<<"void "<<opt.functionName<<"(O& out, P& params) {\n";

	if(!opt.noDocType && opt.docType.empty()) {
		*output<<"\tout<<\"<!DOCTYPE html>\\n\";"<<std::endl;
	} else if(!opt.noDocType) {
		*output<<"\tout<<\"<!DOCTYPE "<<opt.docType<<">\\n\";";
	}

	auto rslt = mainParse(b, e, pos);

	for(auto& it : rslt) {
		it->gen(*output, 0);
	}

	*output<<"}\n";

	return 0;
}
