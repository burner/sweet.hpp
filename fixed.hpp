/* fixed.hpp - a small fixed point datatype consisting of one 64 bit integer
with 3 decimal placed real part Author: Robert "burner" Schadek 
rburners@gmail.com License: LGPL 3 or higher
*/
#pragma once

#include <stdint.h>
#include <math.h>
#include <type_traits>
#include <ostream>
#include <string>
//#include <bitset>

#include <logger.hpp>

namespace sweet { class Fixed; }
std::ostream& operator<<(std::ostream&, const sweet::Fixed&);

namespace sweet {

class Fixed {
	friend std::ostream& ::operator<<(std::ostream&,const sweet::Fixed&);
public:

	//
	// Constructor and assignment operator
	//

	Fixed() : value(0llu) {}

	Fixed(const Fixed&& o) : value(o.value) {
	}

	Fixed(const Fixed& o) : value(o.value) {
	}

	template<typename T>
	Fixed& operator=(T&& t) {
		this->set<T>(t);
		return *this;
	}

	template<typename T>
	Fixed(T t) {
		this->set(t);
	}

	//
	// Operator
	//

	template<typename T>
	Fixed operator+(T t) const {
		return this->add(t);
	}

	template<typename T>
	Fixed& operator+=(T t) {
		*this = this->add(t);
		return *this;
	}

	template<typename T>
	Fixed operator-(T t) const {
		return this->minus(t);
	}

	template<typename T>
	Fixed& operator-=(T t) {
		*this = this->minus(t);
		return *this;
	}

	template<typename T>
	Fixed operator*(T t) const {
		return this->multiply(t);
	}

	template<typename T>
	Fixed& operator*=(T t) {
		*this = this->multiply(t);
		return *this;
	}

	template<typename T>
	Fixed operator/(T t) const {
		return this->multiply(t);
	}

	template<typename T>
	Fixed& operator/=(T t) {
		*this = this->div(t);
		return *this;
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

	//
	// Cast operator
	//
	operator int64_t() const {
		return castImpl<int64_t>();
	}

	operator double() const {
		return castImpl<double>();
	}

	operator long double() const {
		return castImpl<long double>();
	}

private:

	//
	// Cast implementation
	//
	template<typename T>
	T castImpl() const {
		T a(static_cast<T>(this->value));
		T b(static_cast<T>(Fixed::ShiftValue));
		return a / b;
	}

	//
	// setting the value
	//
	template<typename T>
	void set(T t,
		typename std::enable_if<
			std::is_same<typename std::remove_reference<T>::type,Fixed>::value
		>::type* = 0) 
	{
		this->value = t.value;
	}

	template<typename T>
	void set(T t,
		typename std::enable_if<std::is_integral<T>::value >::type* = 0) 
	{
		this->value = static_cast<int64_t>(t) << Fixed::Shift;
	}

	template<typename T>
	void set(T t,
		typename std::enable_if<std::is_floating_point<T>::value >::type* = 0) 
	{

		double tvv = t * Fixed::ShiftValue;
		this->value = tvv + static_cast<int64_t>(tvv + t >= 0 ? 0.5 : -0.5);
	}

	template<typename T>
	void set(T s,
		typename std::enable_if<
			std::is_same<typename 
				std::remove_reference<T>::type,std::string
			>::value
		>::type* = 0) 
	{
		this->setString(s);
	}

	template<typename T>
	void set(T s,
		typename std::enable_if<
			std::is_same<typename 
				std::remove_reference<T>::type,const char*
			>::value
		>::type* = 0) 
	{
		this->setString(s);
	}

	void setString(const std::string& s) {
		auto comma = s.find('.');
		//LOG("%s", s.substr(0,comma));
		this->value = std::stoll(s.substr(0, comma))<<Fixed::Shift;
		if(comma != std::string::npos && comma+1 != s.size()) {
			//LOG("%s", s.substr(comma+1, s.size()));
			this->value += std::stol(s.substr(comma+1, s.size()));
		}
	}

	
	//
	// div
	//
	
	template<typename T>
	Fixed div(T t, 
		typename std::enable_if<std::is_same<Fixed,T>::value >::type* = 0) 
		const 
	{
		Fixed ret = *this;
		ret.value /= t.value;
		return ret;
	}

	template<typename T>
	Fixed div(T t, 
		typename std::enable_if<std::is_integral<T>::value >::type* = 0) 
		const 
	{
		return this->div(Fixed(t));
	}

	template<typename T>
	Fixed div(T t, 
		typename std::enable_if<std::is_floating_point<T>::value >::type* = 0) 
		const 
	{
		return this->div(Fixed(t));
	}
	
	//
	// multiply
	//
	template<typename T>
	Fixed multiply(T t, 
		typename std::enable_if<std::is_same<Fixed,T>::value >::type* = 0) 
		const 
	{
		Fixed ret = *this;
		ret.value *= t.value;
		return ret;
	}

	template<typename T>
	Fixed multiply(T t, 
		typename std::enable_if<std::is_integral<T>::value >::type* = 0) 
		const 
	{
		return this->multiply(Fixed(t));
	}

	template<typename T>
	Fixed multiply(T t, 
		typename std::enable_if<std::is_floating_point<T>::value >::type* = 0) 
		const 
	{
		return this->multiply(Fixed(t));
	}

	//
	// add
	//
	template<typename T>
	Fixed add(T t, 
		typename std::enable_if<std::is_same<Fixed,T>::value >::type* = 0) 
		const 
	{
		Fixed r;
		r.value = this->value + t.value;
		return r;
	}

	template<typename T>
	Fixed add(T t, 
		typename std::enable_if<std::is_integral<T>::value >::type* = 0) 
		const 
	{
		return this->add(Fixed(t));
	}

	template<typename T>
	Fixed add(T t, 
		typename std::enable_if<std::is_floating_point<T>::value >::type* = 0) 
		const 
	{
		return this->add(Fixed(t));
	}

	//
	// minus
	//
	template<typename T>
	Fixed minus(T t, 
		typename std::enable_if<std::is_same<Fixed,T>::value >::type* = 0) 
		const 
	{
		Fixed r;
		r.value = this->value - t.value;
		return r;
	}

	template<typename T>
	Fixed minus(T t, 
		typename std::enable_if<std::is_integral<T>::value >::type* = 0) 
		const 
	{
		return this->minus(Fixed(t));
	}

	template<typename T>
	Fixed minus(T t, 
		typename std::enable_if<std::is_floating_point<T>::value >::type* = 0) 
		const 
	{
		return this->minus(Fixed(t));
	}

	//
	// comparision
	//
	template<typename T>
	int compare(T t,
		typename std::enable_if<
			std::is_same<typename std::remove_reference<T>::type,Fixed>::value
		>::type* = 0) const
	{
		return this->value < t.value ? - 1 : this->value == t.value ? 0 : 1;
	}

	template<typename T>
	int compare(T t, 
		typename std::enable_if<std::is_integral<T>::value >::type* = 0) 
		const 
	{
		return this->compare(Fixed(t));

	}

	template<typename T>
	int compare(T t, 
		typename std::enable_if<std::is_floating_point<T>::value >::type* = 0) 
		const 
	{
		return this->compare(Fixed(t));
	}

	//
	// Data member
	//

public:
	static const int Shift = 32;
	static const int64_t ShiftValue = (1llu<<Shift);
	static const int64_t ShiftValueMinus1 = ShiftValue-1;
	static const int64_t ClearTop = 0x00000000FFFFFFFF;
	static const int64_t ClearBottom = 0xFFFFFFFF00000000;

private:
	int64_t value;
};

}

std::ostream& operator<<(std::ostream& os, const sweet::Fixed& d) {
	os<<(d.value >> 32u)<<'.'<<(d.value & sweet::Fixed::ClearTop);
	return os;
}
