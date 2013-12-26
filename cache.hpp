#pragma once

#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <type_traits>

namespace sweet {

//
// some traits
//
template <typename T>
struct is_vector : std::false_type { };
template <typename... Ts> 
struct is_vector<std::vector<Ts...> > : std::true_type { };

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
constexpr size_t SizeOf(const T& t, 
		typename std::enable_if<is_vector<T>::value, T>::type* = 0) {
	return sizeof(typename T::value_type) * t.capacity() + sizeof(T);
}

template<typename T>
constexpr size_t SizeOf(const T& t, 
		typename std::enable_if<is_list<T>::value, T>::type* = 0) {
	return sizeof(typename T::value_type) * t.size() + sizeof(T) +
		t.size() * 2u * sizeof(size_t);
}

template<typename T>
constexpr size_t SizeOf(const T& t, 
		typename std::enable_if<is_map<T>::value || is_set<T>::value, T>
		::type* = 0) {
	return sizeof(typename T::value_type) * t.size() + sizeof(T) +
		t.size() * 3u * sizeof(size_t);
}

template<typename T>
constexpr size_t SizeOf(const T& t, 
		typename std::enable_if<is_unordered_map<T>::value || 
		is_unordered_set<T>::value, T>::type* = 0) {
	return sizeof(typename T::value_type) * t.size() + sizeof(T) +
		t.size() * 3u * sizeof(size_t);
}

template<typename T>
constexpr size_t SizeOf(const T&,
		typename std::enable_if<
				is_vector<T>::value == false && 
				is_list<T>::value == false && 
				is_associated<T>::value == false,
			 T>::type* = 0) {
   	return sizeof(T);
}

//
// the cache implementation
//
template<typename K, typename V, size_t CacheSize>
class cache {
private:

};

}
