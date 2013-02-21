#ifndef CONV_HPP
#define CONV_HPP
#include <type_traits>

template<typename T, typename S, typename R>
struct toImpl<T,S,R> {
	T operator()(S s) {
		return static_cast<T>(s);
	}
};

template<typename T, typename S>
struct toImpl<T,S,true> {
	T operator()(S s) {
		return static_cast<T>(s);
	}
};

template<typename T, typename S>
float to(S s) {
	return toImpl<T,S,is_floating_point<T>::value && isInteger<S>::value>()(s);
}
#endif
