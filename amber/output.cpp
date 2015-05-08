#include "amber.hpp"

static void createIndent(std::ostream& out, const size_t indent) {
	for(size_t i = 0; i < indent; ++i) {
		out<<'\t';	
	}
}

void Node::gen(std::ostream& out, const size_t indent) { 
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

	if(this->openClose) {
		out<<"/>\n";
		return;
	}

	out<<'>'<<'\n';
	for(auto& it : children) {
		it->gen(out, indent+1);
	}

	createIndent(out, indent);
	out<<'<'<<this->type<<"/>"<<'\n';
}

void CNode::gen(std::ostream& out, const size_t) { 
	out<<this->program<<'\n';
}

void TNode::gen(std::ostream& out, const size_t indent) { 
	createIndent(out, indent);
	auto be = this->line.begin();
	auto en = this->line.end();

	if(!test(be, en, "&{{") && test(be, en, '&')) {
		++be;
		while(be != en && ( *be == ' ' || *be == '\t')) {
			++be;
		}
		std::copy( be, en, std::ostream_iterator<char>(out));
	} else {
		std::copy( be, en, std::ostream_iterator<char>(out));
	}
	out<<'\n';
}
