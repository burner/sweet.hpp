#pragma once

#include <stdint.h>
#include <type_traits>

#include <logger.hpp>

class Decimal {
public:
	template<typename T>
	Decimal add(T t, typename std::enable_if<std::is_same<Decimal,T>::value >::type* = 0) const {
		Decimal ret = *this;
		return Decimal();
	}

	template<typename T>
	Decimal add(T t, typename std::enable_if<std::is_integral<T>::value >::type* = 0) const {
		Decimal ret = *this;
		ret.fixed += t;
		return Decimal();
	}

	template<typename T>
	Decimal add(T t, typename std::enable_if<std::is_floating_point<T>::value >::type* = 0) const {
		return Decimal();
	}

	template<typename T>
	Decimal operator+(T t) const {
		return this->add(t);
	}

	template<typename T>
	Decimal& operator+=(T t) {
		*this = this->add(t);
		return *this;
	}

private:
	int64_t fixed;
	int64_t fraction;
};
