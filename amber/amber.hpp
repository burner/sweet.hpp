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

struct TNode : public AstBase {
	TNode(std::string&& p, const Pos& po);

	void gen(std::ostream& out, const size_t) override;

	std::string line;
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

