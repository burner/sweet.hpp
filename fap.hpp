#pragma once

#include <fector.hpp>

namespace sweet {

template<typename K, typename V, size_t Size = 128, typename Compare = std::less<K>>
class Fap {
public:
	typedef std::pair<K,V> value_type;
	typedef std::pair<const K,V> const_value_type;
	typedef const_value_type& reference;
	typedef const_value_type& const_reference;
	typedef const_value_type* pointer;
	typedef const const_value_type* const_pointer;
	typedef const_value_type* iterator;
	typedef const const_value_type* const_iterator;

private:
	Compare less;
	auto iteratorCast(typename Fector<value_type>::iterator it) {
		return reinterpret_cast<iterator>(it);
	}

	auto iteratorCast(typename Fector<value_type>::const_iterator it) {
		return reinterpret_cast<const_iterator>(it);
	}

	auto pointerCast(typename Fector<value_type>::pointer it) {
		return reinterpret_cast<pointer>(it);
	}

	auto pointerCast(typename Fector<value_type>::const_pointer it) {
		return reinterpret_cast<const_pointer>(it);
	}

	auto referenceCast(typename Fector<value_type>::reference it) {
		return reinterpret_cast<reference>(it);
	}

	auto referenceCast(typename Fector<value_type>::const_reference it) {
		return reinterpret_cast<const_reference>(it);
	}

public:

	inline std::pair<iterator,bool> insert(const value_type& v) {
		auto it = std::lower_bound(map.begin(), map.end(), v, [&](auto a, auto b) {
			return less(a.first,b.first);
		});

		if(it == this->map.end()) {
			this->map.push_back(v);
			return std::make_pair(iteratorCast(&this->map.back()), true);
		} else if(it->first != v.first) {
			it = this->map.insert(it, v);
			return std::make_pair(iteratorCast(it), true);
		} else {
			return std::make_pair(iteratorCast(it), false);
		}
	}

	// search
	
	iterator find(const K& key) {
		auto it = std::lower_bound(map.begin(), map.end(), key, [&](auto a, auto b) {
			return less(a.first,b);
		});

		if(it == this->map.end()) {
			return iteratorCast(it);
		} else if(!less(it->first, key)) {
			return iteratorCast(it);
		} else {
			return iteratorCast(this->map.end());
		}
	}

	// iterator

	iterator begin() {
		return iteratorCast(this->map.begin());
	}

	iterator end() {
		return iteratorCast(this->map.end());
	}

	const_iterator begin() const {
		return iteratorCast(this->map.begin());
	}

	const_iterator end() const {
		return iteratorCast(this->map.end());
	}

	iterator rbegin() {
		return iteratorCast(this->map.rbegin());
	}

	iterator rend() {
		return iteratorCast(this->map.rend());
	}

	const_iterator rbegin() const {
		return iteratorCast(this->map.rbegin());
	}

	const_iterator rend() const {
		return iteratorCast(this->map.rend());
	}

private:
	Fector<value_type, Size> map;
};


} // namespace sweet
