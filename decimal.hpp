/* decimal.hpp - a small fixed point datatype consisting of two 64 bit integers
 C++ Author: Robert "burner" Schadek rburners@gmail.com License: LGPL 3 or higher
*/
#pragma once

#include <stdint.h>
#include <math.h>
#include <type_traits>
#include <ostream>

#include <logger.hpp>

namespace sweet {

class Decimal {
	//friend std::ostream& operator<<(std::ostream&,const Decimal&);
public:

	//
	// Constructor and assignment operator
	//

	Decimal() : fixed(0llu), fraction(0llu) {}

	Decimal(const Decimal&& o) : fixed(o.fixed), fraction(o.fraction) {
	}

	Decimal(const Decimal& o) : fixed(o.fixed), fraction(o.fraction) {
	}

	template<typename T>
	Decimal& operator=(T&& t) {
		this->set<T>(t);
		return *this;
	}

	template<typename T>
	Decimal(T t) {
		this->set(t);
	}

	//
	// Operator
	//

	template<typename T>
	Decimal operator+(T t) const {
		return this->add(t);
	}

	template<typename T>
	Decimal& operator+=(T t) {
		*this = this->add(t);
		return *this;
	}

	template<typename T>
	Decimal operator-(T t) const {
		return this->minus(t);
	}

	template<typename T>
	Decimal& operator-=(T t) {
		*this = this->minus(t);
		return *this;
	}

	template<typename T>
	Decimal operator*(T t) const {
		return this->multiply(t);
	}

	//
	// Comparision
	//

	template<typename T>
	bool operator==(T t) const {
		return this->compare(t) == 0;
	}

	template<typename T>
	bool operator<(T t) const {
		return this->compare(t) == -1;
	}

	template<typename T>
	bool operator>(T t) const {
		return this->compare(t) == 1;
	}

//private:

	//
	// setting
	//
	template<typename T>
	void set(T t,
		typename std::enable_if<
			std::is_same<typename std::remove_reference<T>::type,Decimal>::value
		>::type* = 0) 
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
		this->fraction = static_cast<int64_t>(fmod(t, 1.0) * 
			Decimal::fractionLength
		);
	}
	
	//
	// multiply
	//
	template<typename T>
	Decimal multiply(T t, 
		typename std::enable_if<std::is_same<Decimal,T>::value >::type* = 0) 
		const 
	{
		Decimal ret(*this);
		return ret;
	}

	template<typename T>
	Decimal multiply(T t, 
		typename std::enable_if<std::is_integral<T>::value >::type* = 0) 
		const 
	{
		Decimal ret = *this;
		ret.fixed += t;
		return ret;
	}

	template<typename T>
	Decimal multiply(T t, 
		typename std::enable_if<std::is_floating_point<T>::value >::type* = 0) 
		const 
	{
		Decimal ret(t);
		return ret + *this;
	}

	//
	// add
	//
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

	//
	// minus
	//
	template<typename T>
	Decimal minus(T t, 
		typename std::enable_if<std::is_same<Decimal,T>::value >::type* = 0) 
		const 
	{
		Decimal ret(*this);
		ret.fixed -= t.fixed;
		ret.fraction -= t.fraction;
		if(ret.fraction < 0u) {
			ret.fraction = fractionLength + ret.fraction;
		}
		return ret;
	}

	template<typename T>
	Decimal minus(T t, 
		typename std::enable_if<std::is_integral<T>::value >::type* = 0) 
		const 
	{
		Decimal ret = *this;
		ret.fixed -= t;
		return ret;
	}

	template<typename T>
	Decimal minus(T t, 
		typename std::enable_if<std::is_floating_point<T>::value >::type* = 0) 
		const 
	{
		Decimal ret(t);
		return this->minus(ret);
	}

	//
	// comparision
	//
	template<typename T>
	int compare(T t,
		typename std::enable_if<
			std::is_same<typename std::remove_reference<T>::type,Decimal>::value
		>::type* = 0) const
	{
		int fi = this->fixed == t.fixed ? 0 : 
				this->fixed < t.fixed ? -1 : 1;

		int fa = this->fraction == t.fraction ? 0 : 
				this->fraction < t.fraction ? -1 : 1;

		return fi == 0 ? fa : fi;
	}

	template<typename T>
	int compare(T t, 
		typename std::enable_if<std::is_integral<T>::value >::type* = 0) 
		const 
	{
		Decimal d(t);
		return this->compare(d);

	}

	template<typename T>
	int compare(T t, 
		typename std::enable_if<std::is_floating_point<T>::value >::type* = 0) 
		const 
	{
		Decimal d(t);
		return this->compare(d);
	}

	//
	// Data member
	//

	int64_t fixed;
	int64_t fraction;

	static const int64_t fractionLength = 100000000000000000;
};

inline std::ostream& operator<<(std::ostream& os, const sweet::Decimal& d) {
	os<<d.fixed<<'.'<<d.fraction;
	return os;
}

}
