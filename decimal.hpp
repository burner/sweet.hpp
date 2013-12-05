#pragma once

#include <stdint.h>
#include <math.h>
#include <type_traits>
#include <ostream>

#include <logger.hpp>

class Decimal {
	friend std::ostream& operator<<(std::ostream&,const Decimal&);
public:
	Decimal() : fixed(0llu), fraction(0llu) {}

	Decimal(const Decimal&& o) : fixed(o.fixed), fraction(o.fraction) {
	}

	Decimal(const Decimal& o) : fixed(o.fixed), fraction(o.fraction) {
	}

	Decimal& operator=(const Decimal&& o) {
	   	this->fixed = o.fixed; 
		this->fraction = o.fraction;
		return *this;
	}

	Decimal& operator=(const Decimal& o) {
	   	this->fixed = o.fixed; 
		this->fraction = o.fraction;
		return *this;
	}

	template<typename T>
	Decimal(T t) {
		this->set(t);
	}

	template<typename T>
	void set(T t,
		typename std::enable_if<std::is_same<Decimal,T>::value >::type* = 0) 
	{
		this->fixed = t.fixed;
		this->fraction = t.fraction;
	}

	template<typename T>
	void set(T t,
		typename std::enable_if<std::is_integral<T>::value >::type* = 0) 
	{
		this->fixed = t;
		this->fraction = 0llu;
	}

	template<typename T>
	void set(T t,
		typename std::enable_if<std::is_floating_point<T>::value >::type* = 0) 
	{
		this->fixed = static_cast<int64_t>(t);
		this->fraction = static_cast<int64_t>(fmod(t, 1.0)) * this->fractionLength;
	}

	template<typename T>
	Decimal add(T t, 
		typename std::enable_if<std::is_same<Decimal,T>::value >::type* = 0) 
		const 
	{
		Decimal ret(*this);
		ret.fixed += t.fixed;
		ret.fraction += t.fraction;
		if(ret.fraction > fractionLength) {
			ret.fraction -= fractionLength;
			++ret.fixed;
		}
		return ret;
	}

	template<typename T>
	Decimal add(T t, 
		typename std::enable_if<std::is_integral<T>::value >::type* = 0) 
		const 
	{
		Decimal ret = *this;
		ret.fixed += t;
		return ret;
	}

	template<typename T>
	Decimal add(T t, 
		typename std::enable_if<std::is_floating_point<T>::value >::type* = 0) 
		const 
	{
		Decimal ret(t);
		return ret + *this;
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

	static const int64_t fractionLength = 100000000000000000;
};

std::ostream& operator<<(std::ostream& os, const Decimal& d) {
	os<<d.fixed<<'.'<<d.fraction;
	return os;
}
