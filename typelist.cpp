#include <type_traits>
#include <iostream>

//template<bool Value> struct bool_constant : std::integral_constant<bool, Value> {};
//template<int Value> struct int_constant : std::integral_constant<int, Value> {};
//template<typename Type> struct type_holder { typedef Type type; };

struct typelist_nil;
template<typename Head, class Tail> struct typelist_pair;
template<typename... Types> struct typelist_make;
//template<class TypeList> struct typelist_null;
//template<class TypeList> struct typelist_length;
//template<class TypeList1, class TypeList2> struct typelist_append;
//template<class TypeList> struct typelist_hd;
//template<class TypeList> struct typelist_tl;
//template<class TypeList> struct typelist_last;
//template<class TypeList, int idx> struct typelist_nth;
//template<class TypeList> struct typelist_rev;
//template<template<typename> class F, class TypeList> struct typelist_map;
//template<template<typename, typename> class F, typename Init, class TypeList>
//struct typelist_foldl;
//template<template<typename, typename> class F, typename Init, class TypeList>
//struct typelist_foldr;
//template<template<typename> class F, class TypeList> struct typelist_exists;
//template<template<typename> class F, class TypeList> struct typelist_all;
//template<int Length, template<int> class F> struct typelist_tabulate;
//
//template<template<typename...> class F, class TypeList> struct typelist_template;
//template<class TypeList> struct typelist_inherit;
//template<typename... Types> struct typelist_basetuple;

//typelist_nil

struct typelist_nil {
	static constexpr bool is_nil = true;
};

// typelist_pair
template<typename Head, class Tail>
struct typelist_pair {
	typedef Head head;
	typedef Tail tail;
	static constexpr bool is_nil = false;
};

// typelist_make
template<>
struct typelist_make<> : typelist_nil {};

template<typename Head, typename...  TailTypes>
struct typelist_make<Head, TailTypes...> 
: typelist_pair<Head,
	typelist_make<TailTypes...>> {};
