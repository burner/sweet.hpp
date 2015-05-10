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
	/*auto be = str.find("&{{");
	decltype(be) en = std::string::npos;

	createIndent(out, indent);
	Beg(out)<<'"';
	std::copy(str.begin(), 
		(be != std::string::npos ? str.begin()+be : str.end()), 
		std::ostream_iterator<char>(out));
	End(out);

	while(be != std::string::npos) {
		en = str.find("}}&", be);

		if(en == std::string::npos) {
			throw std::logic_error("Embedded C++ expressing "
				"not correctly ended");
		}
		
		createIndent(out, indent);
		out<<"out<<(";
		std::copy(str.begin()+be+3, str.begin()+en, 
			std::ostream_iterator<char>(out));
		out<<");\n";

		be = en + 3u;

		be = str.find("&{{", en);
	}

	//std::copy(str.begin()+be, str.begin()+en, 
	//	std::ostream_iterator<char>(out));
	*/
	auto be = str.begin();
	auto en = str.end();

	while(be != en) {
		auto ben = be;
		for(; ben != en && *ben != '&'; ++ben) {}

		out<<"out<<\"";
		std::copy(be, ben, std::ostream_iterator<char>(out));
		out<<"\"";
		break;

		//be = ben;
		//if(test(be, en, "&{{")) {
		//	be += 3u;

		//	ben = be;
		//	for(; ben != en && test(ben,en,"}}&"); ++ben) {}

		//	out<<"out<<(";
		//	std::copy(be, ben, std::ostream_iterator<char>(out));
		//	out<<");\\n"<<"\n";

		//	be += 3;
		//}

		//++be;
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
	createIndent(out, indent);

	if(!test(be, en, "&{{") && test(be, en, '&')) {
		++be;
		eatWhitespace(be, en, pos);
		std::copy(be, en, std::ostream_iterator<char>(out));
	} else {
		formatNormalLine(out, this->line, indent+1);
	}
	out<<'\n';
}
