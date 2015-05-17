// Copyright: Robert "burner" Schadek rburners@gmail.com License: GPL 3 or higher
#pragma once
#include <algorithm>
#include <assert.h>
#include <iterator>
#include <ostream>
#include <string>
#include <memory>
#include <vector>

struct Pos {
	size_t row;
	size_t column;

	Pos();
};

struct AstBase {
	AstBase(const Pos&);
	virtual void gen(std::ostream&, const size_t) = 0;

	Pos pos;
};

typedef std::unique_ptr<AstBase> NPtr;
typedef std::vector<NPtr> Children;

struct Node : public AstBase {
	Node(std::string&& t, std::string&& cl, std::string&& i, 
		std::string&& a, Children&& c, const Pos& p);

	Node(std::string&& t, std::string&& cl, std::string&& i, 
		std::string&& a, const Pos& p);

	void gen(std::ostream& out, const size_t indent) override;

	bool openClose;
	std::string classLit;
	std::string idLit;
	std::string attributes;
	std::string type;
	Children children;
};

struct CNode : public AstBase {
	CNode(std::string&& p, const Pos& po);

	void gen(std::ostream& out, const size_t) override;

	std::string program;
};

enum class TNodeType {
	Text,
	SingleCppLine,
	Include
};

struct TNode : public AstBase {
	TNode(std::string&& p, const Pos& po, const TNodeType t);

	void gen(std::ostream& out, const size_t) override;

	std::string line;
	TNodeType type;
};

//void createIndent(std::ostream& out, const size_t indent);
template<typename I>
bool test(I be, I en, const char tt) {
	if(be != en && *be == tt) {
		return true;
	} else {
		return false;
	}
}

template<typename I>
bool test(I be, I en, const std::string& tt) {
	assert(be <= en);
	if(static_cast<std::size_t>(distance(be, en)) >= tt.size() && 
		std::equal(be, be+tt.size(), tt.begin(), tt.end())) 
	{
		return true;
	} else {
		return false;
	}
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
I eatUntil(I& be, I& en, const std::string& until, Pos& pos) {
	auto it = be;

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
void eatWhitespaceComment(I& be, I& en, Pos& pos) {
	while(true) {
		if(test(be, en, "//")) {
			increment(be, pos, 2u);
			be = eatUntil(be, en, "\n", pos);
			eatWhitespace(be, en, pos);
		} else if(std::isspace(*be)) {
			eatWhitespace(be, en, pos);
		} else {
			break;
		}
	}
}

struct CmdOptions {
	std::string input;
	std::string output;
	std::string functionName;
	std::string docType;

	bool allOk;
};
