#include "amber.hpp"

static void createIndent(std::ostream& out, const size_t indent) {
	for(size_t i = 0; i < indent; ++i) {
		out<<'\t';	
	}
}

static std::ostream& Beg(std::ostream& out) {
	out<<"out<<\"";
	return out;
}

static std::ostream& End(std::ostream& out) {
	out<<"\";\n";
	return out;
}

static void formatNormalLine(std::ostream& out, const std::string& str, 
		const size_t indent) 
{
	size_t i = 0;
	const size_t ss = str.size();

	while(true) {
		auto b = str.find("&{{", i);

		b = (b == std::string::npos ? ss : b);

		if(b > i) {
			//createIndent(out, indent);
			auto iter = str.begin() + i;
			auto iterB = str.begin() + b;
			while(iter != iterB && std::isspace(*iter)) {
				++iter;
			}
			Beg(out);
			assert(!std::isspace(*iter));
			std::copy(iter, iterB, 
				std::ostream_iterator<char>(out));
			End(out);
		}

		if(b == ss) {
			break;
		} else {
			auto e = str.find("}}&", b);
			assert(e != std::string::npos);

			createIndent(out, indent);
			auto iter = str.begin() + b + 3;
			auto iterB = str.begin() + e;
			out<<"out<<(";
			//std::copy(str.begin() + b + 3, str.begin() + e, 
			std::copy(iter, iterB, 
				std::ostream_iterator<char>(out));
			out<<");\n";

			i = e + 3u;
		}
	}
}

void Node::gen(std::ostream& out, const size_t indent) { 
	createIndent(out, indent);
	Beg(out)<<'<'<<this->type;
	if(!this->idLit.empty()) {
		out<<" id=\""<<this->idLit<<'"';
	}
	if(!this->classLit.empty()) {
		out<<" class=\""<<this->classLit<<'"';
	}
	if(!this->attributes.empty()) {
		out<<' '<<this->attributes;
	}

	if(this->openClose) {
		out<<"/>";
		End(out);
		return;
	}

	out<<">\\n";
   	End(out);
	for(auto& it : children) {
		it->gen(out, indent+1);
	}

	createIndent(out, indent);
	Beg(out)<<'<'<<this->type<<"/>\\n";
	End(out);
}

void CNode::gen(std::ostream& out, const size_t) { 
	out<<this->program<<'\n';
}

void TNode::gen(std::ostream& out, const size_t indent) { 
	Pos pos;
	auto be = this->line.begin();
	auto en = this->line.end();

	eatWhitespace(be, en, pos);

	if(!test(be, en, "&{{") && test(be, en, '&')) {
		++be;
		eatWhitespace(be, en, pos);
		createIndent(out, indent + 1);
		std::copy(be, en, std::ostream_iterator<char>(out));
	} else {
		formatNormalLine(out, this->line, indent+1);
	}
	out<<'\n';
}
