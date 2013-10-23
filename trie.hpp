#ifndef SWEET_TRIE_HPP
#define SWEET_TRIE_HPP
//#include <unordered_map>
#include <map>
#include <ostream>

template<typename K, typename V, typename C>
class Trie;

template<typename K, typename V, typename C>
class Entry;

template<typename K, typename V, typename C>
std::ostream& operator<<(std::ostream&, const Trie<K,V,C>&);

template<typename K, typename V, typename C>
std::ostream& operator<<(std::ostream&, const Entry<K,V,C>&);

template<typename K, typename V, typename C>
struct Entry {
	friend std::ostream& operator<< <>(std::ostream&, const Entry<K,V,C>&);
	std::map<K,Entry,C> map;
	V value;
	bool isValue;
	int32_t depth;

	inline Entry() : isValue(false), depth(0) {}
	inline Entry(const int32_t d) : isValue(false), depth(d) {}
};

template<typename K, typename V, typename C = std::less<K>>
class Trie {
public:
	typedef Entry<K,V,C> TrieEntry;

private:
	TrieEntry entries;

public:
	friend std::ostream& operator<< <>(std::ostream&, const Trie<K,V,C>&);
	Trie() : entries(0) {}

	template<typename B, typename E>
	bool insert(B first, E end, V value) {
		TrieEntry* cur = &entries;
		for(; first != end; ++first) {
			auto next = cur->map.find(*first);
			if(next == cur->map.end()) {
				cur->map.insert(std::make_pair(*first, TrieEntry(cur->depth+1)));
			}
			next = cur->map.find(*first);
			cur = &(next->second);
		}
		if(cur->isValue) {
			return false;
		} else {
			cur->value = value;
			cur->isValue = true;
			return true;
		}
	}

	inline TrieEntry& getRoot() {
		return entries;
	}

	inline const TrieEntry& getRoot() const {
		return entries;
	}
};

template<typename K, typename V, typename C>
std::ostream& operator<<(std::ostream& os, const Trie<K,V,C>& t) {
	os<<"Trie:";
	os<<t.getRoot();
	return os;
}

template<typename K, typename V, typename C>
std::ostream& operator<<(std::ostream& os, const Entry<K,V,C>& t) {
	if(t.isValue) {
		os<<t.value;
	}
	os<<std::endl;
	for(auto& it : t.map) {
		os<<std::string(it.second.depth*2, ' ')<<it.first<<":"<<it.second;
	}
	return os;
}

#endif
