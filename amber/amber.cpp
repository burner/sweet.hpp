#include <iostream>
#include <fstream>
#include <istream>
#include <ostream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <utility>
#include <cctype>
#include <stdexcept>

struct Pos {
	size_t row;
	size_t column;

	inline Pos() : row(1), column(1) {}
};

template<typename I>
void increment(I& iter, Pos& pos, const size_t cnt = 1) {
	if(*iter == '\n') {
		++pos.row;
		pos.column = 1u;
	} else {
		++pos.column;
	}

	iter += cnt;
}

struct AstBase {
	inline AstBase(const Pos& p) : pos(p) {}
	virtual void gen(std::ostream&, const size_t) = 0;

	Pos pos;
};

void createIndent(std::ostream& out, const size_t indent) {
	for(size_t i = 0; i < indent; ++i) {
		out<<'\t';	
	}
}

typedef std::unique_ptr<AstBase> NPtr;
typedef std::vector<NPtr> Children;

struct Node : public AstBase {
	inline Node(std::string&& t, std::string&& cl, std::string&& i, 
		std::string&& a, Children&& c, const Pos& p) : AstBase(p),
   		classLit(std::move(cl)), idLit(std::move(i)), attributes(std::move(a)),
	   	type(std::move(t)), children(std::move(c)) {
	}

	inline void gen(std::ostream& out, const size_t indent) override { 
		createIndent(out, indent);
		out<<'<'<<this->type;
		if(!this->idLit.empty()) {
			out<<" id=\""<<this->idLit<<'"';
		}
		if(!this->classLit.empty()) {
			out<<" class=\""<<this->classLit<<'"';
		}
		if(!this->attributes.empty()) {
			out<<' '<<this->attributes;
		}
		out<<'>'<<'\n';
		for(auto& it : children) {
			it->gen(out, indent+1);
		}

		createIndent(out, indent);
		out<<'<'<<this->type<<"/>"<<'\n';
	}

	std::string classLit;
	std::string idLit;
	std::string attributes;
	std::string type;
	Children children;
};

struct CNode : public AstBase {
	inline CNode(std::string&& p, const Pos& po) : AstBase(po), program(std::move(p)) {
	}
	inline void gen(std::ostream& out, const size_t) override { 
		out<<this->program<<'\n';
	}

	std::string program;
};

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
NPtr parseC(I& be, I& en, Pos& pos) {
	eat(be, "<{{", pos);

	auto beCopy = be;

	for(; beCopy != en; increment(beCopy, pos)) {
		if(*beCopy == '}' && 
				distance(beCopy, en) >= 3 && std::string(beCopy, beCopy+3) == "}}>") 
		{

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
	for(; beCopy != en && std::isalpha(*beCopy); increment(beCopy, pos)) {}

	auto type = std::string(be, beCopy);

	be = beCopy;

	std::string classLit; // .
	std::string idLit;    // #
	std::string attributes; // ( **** )

	while(*be == '.' || *be == '#') {
		beCopy = be;
		increment(beCopy, pos);
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

		be=beCopy;
		eat(be, ')', pos);
	}

	eatWhitespace(be, en, pos);

	auto children = mainParse(be, en, pos);

	return std::move(std::make_unique<Node>(std::move(type), 
		std::move(classLit), std::move(idLit), std::move(attributes),
		std::move(children), pos));
}

template<typename I>
Children mainParse(I& be, I& en,  Pos& pos) {
	Children ret;

	while(be != en) {
		eatWhitespace(be, en, pos);

		if(distance(be, en) >= 3u && std::string(be, be+3) == "<{{") {
			ret.push_back(std::move(parseC(be, en, pos)));	
		} else if(be != en && *be == '<') {
			ret.push_back(std::move(parseNode(be, en, pos)));
		} else if(be != en && *be == '>') {
			eat(be, '>', pos);
			break;
		}
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
