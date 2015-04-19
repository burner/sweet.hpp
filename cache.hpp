// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
#pragma once

#include <algorithm>
#include <cassert>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector> 

namespace sweet {

//
// some traits
//
template <typename T>
struct is_vector : std::false_type { };
template <typename... Ts> 
struct is_vector<std::vector<Ts...> > : std::true_type { };

template <typename T>
struct is_deque : std::false_type { };
template <typename... Ts> 
struct is_deque<std::deque<Ts...> > : std::true_type { };

template <typename T>
struct is_list : std::false_type { };
template <typename... Ts> 
struct is_list<std::list<Ts...> > : std::true_type { };

template <typename T>
struct is_map : std::false_type { };
template <typename... Ts> 
struct is_map<std::map<Ts...> > : std::true_type { };

template <typename T>
struct is_set : std::false_type { };
template <typename... Ts> 
struct is_set<std::set<Ts...> > : std::true_type { };

template <typename T>
struct is_unordered_map : std::false_type { };
template <typename... Ts> 
struct is_unordered_map<std::unordered_map<Ts...> > : std::true_type { };

template <typename T>
struct is_unordered_set : std::false_type { };
template <typename... Ts> 
struct is_unordered_set<std::unordered_set<Ts...> > : std::true_type { };

template <typename T>
struct is_associated : std::false_type { };

template <typename... Ts> 
struct is_associated<std::map<Ts...> > : std::true_type { };
template <typename... Ts> 
struct is_associated<std::set<Ts...> > : std::true_type { };
template <typename... Ts> 
struct is_associated<std::unordered_map<Ts...> > : std::true_type { };
template <typename... Ts> 
struct is_associated<std::unordered_set<Ts...> > : std::true_type { };

template<typename T>
size_t SizeOf(const T& t, 
		typename std::enable_if<is_vector<T>::value, T>::type* = 0) {
	return sizeof(typename T::value_type) * t.capacity() + sizeof(T);
}

template<typename T>
size_t SizeOf(const T& t, 
		typename std::enable_if<is_deque<T>::value, T>::type* = 0) {
	return sizeof(typename T::value_type) * t.size() + sizeof(T);
}

template<typename T>
size_t SizeOf(const T& t, 
		typename std::enable_if<is_list<T>::value, T>::type* = 0) {
	return sizeof(typename T::value_type) * t.size() + sizeof(T) +
		t.size() * 2u * sizeof(size_t);
}

template<typename T>
size_t SizeOf(const T& t, 
		typename std::enable_if<is_map<T>::value || is_set<T>::value, T>
		::type* = 0) {
	return sizeof(typename T::value_type) * t.size() + sizeof(T) +
		t.size() * 3u * sizeof(size_t);
}

template<typename T>
size_t SizeOf(const T& t, 
		typename std::enable_if<is_unordered_map<T>::value || 
		is_unordered_set<T>::value, T>::type* = 0) {
	return sizeof(typename T::value_type) * t.size() + sizeof(T) +
		t.size() * 3u * sizeof(size_t);
}

template<typename T>
constexpr size_t SizeOf(const T&,
		typename std::enable_if<
				is_deque<T>::value == false && 
				is_vector<T>::value == false && 
				is_list<T>::value == false && 
				is_associated<T>::value == false,
			 T>::type* = 0) {
   	return sizeof(T);
}

//
// the Cache implementation
//
template<typename K, typename V, size_t CacheSize>
class Cache {
public:
	Cache() : memInBytes(0u) {
	}

	typedef std::pair<K,V> ListEntry;
	typedef std::list<ListEntry> List;
	typedef std::unordered_map<K,typename List::iterator> Map;

	void insert(const K& key, V value) {
		this->insert(key, value, [](const K&, V&&) {});
	}

	void insert(const K& key, V value, 
			std::function<void(const K&, V&&)> saveFunc) 
	{
		size_t bytesOfNew(SizeOf(value));
		if(this->memInBytes + bytesOfNew > CacheSize && !this->list.empty()) {
			auto toDel(this->list.back());
			this->list.pop_back();

			auto mapIt(this->map.find(toDel.first));
			assert(mapIt != this->map.end());
			this->map.erase(mapIt);
			saveFunc(toDel.first, std::move(toDel.second));
		}

		this->list.push_front(std::make_pair(key, value));
		this->map.insert(std::make_pair(key, this->list.begin()));

		this->memInBytes = this->sizeOfSavedElementsInBytes();
	}

	bool erase(const typename Map::iterator& it) {
		if(it != this->map.end()) {
			return this->erase(it->first);
		}
		return false;
	}

	bool erase(const K& key) {
		return this->erase(key, [](const K&, V&&) {});
	}

	bool erase(const K& key, std::function<void(const K&, V&&)> saveFunc) {
		auto mapIt(this->map.find(key));
		if(mapIt != this->map.end()) {
			saveFunc(key,std::move(mapIt->second->second));
			this->list.erase(mapIt->second);
			this->map.erase(mapIt);
			return true;
		}
		return false;
	}

	size_t size() const {
		assert(this->list.size() == this->map.size());
		return this->list.size();
	}

	size_t bytesStored() const {
		return this->memInBytes;
	}

	typename Map::iterator find(const K& key) {
		auto ret(this->map.find(key));
		if(ret != this->map.end()) {
			this->list.splice(this->list.begin(), this->list, ret->second);
		}
		return ret;
	}

	typename Map::iterator end() {
		return this->map.end();
	}

	bool contains(const K& key) {
		return this->find(key) != this->end();
	}

private:
	size_t sizeOfSavedElementsInBytes() const {
		size_t ret(0);
		std::for_each(list.cbegin(), list.cend(), [&ret](const ListEntry& v) {
			ret += SizeOf(v.second);
		});
		return ret;
	}

	size_t memInBytes;
	Map map;
	List list;
};

}
