/* fixed.hpp - a small fixed point datatype consisting of one 64 bit integer
with 32 bit for the decimal and real part Author: Robert "burner" Schadek 
rburners@gmail.com License: LGPL 3 or higher
*/
#pragma once

#include <stdint.h>
#include <math.h>
#include <type_traits>
#include <ostream>
#include <string>

namespace sweet { class Fixed; }
std::ostream& operator<<(std::ostream&, const sweet::Fixed&);

namespace sweet {

class Fixed {
	friend std::ostream& ::operator<<(std::ostream&,const sweet::Fixed&);
public:

	//
	// Constructor and assignment operator
	//

	Fixed() noexcept : value(0llu) {}

	Fixed(const Fixed&& o) noexcept : value(o.value) {
	}

	Fixed(const Fixed& o) noexcept : value(o.value) {
	}

	template<typename T>
	Fixed& operator=(T&& t) noexcept {
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
	Fixed operator+(T t) const noexcept {
		return this->add(t);
	}

	template<typename T>
	Fixed& operator+=(T t) {
		*this = this->add(t);
		return *this;
	}

	template<typename T>
	Fixed operator-(T t) const noexcept {
		return this->minus(t);
	}

	template<typename T>
	Fixed& operator-=(T t) noexcept {
		*this = this->minus(t);
		return *this;
	}

	template<typename T>
	Fixed operator*(T t) const noexcept {
		return this->multiply(t);
	}

	template<typename T>
	Fixed& operator*=(T t) noexcept {
		*this = this->multiply(t);
		return *this;
	}

	template<typename T>
	Fixed operator/(T t) const noexcept {
		return this->multiply(t);
	}

	template<typename T>
	Fixed& operator/=(T t) noexcept {
		*this = this->div(t);
		return *this;
	}

	//
	// Comparision
	//

	template<typename T>
	bool operator==(T t) const noexcept {
		return this->compare(t) == 0;
	}

	template<typename T>
	bool operator<(T t) const noexcept {
		return this->compare(t) == -1;
	}

	template<typename T>
	bool operator>(T t) const noexcept {
		return this->compare(t) == 1;
	}

	//
	// Cast operator
	//
	operator int64_t() const noexcept {
		return castImpl<int64_t>();
	}

	operator double() const noexcept {
		return castImpl<double>();
	}

	operator long double() const noexcept {
		return castImpl<long double>();
	}

	int64_t getValue() const noexcept {
		return this->value;
	}

private:

	//
	// Cast implementation
	//
	template<typename T>
	T castImpl() const noexcept {
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
		>::type* = 0) noexcept
	{
		this->value = t.value;
	}

	template<typename T>
	void set(T t,
		typename std::enable_if<std::is_integral<T>::value >::type* = 0) 
		noexcept
	{
		this->value = static_cast<int64_t>(t) << Fixed::Shift;
	}

	template<typename T>
	void set(T t,
		typename std::enable_if<std::is_floating_point<T>::value >::type* = 0) 
		noexcept
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
		this->value = std::stoll(s.substr(0, comma))<<Fixed::Shift;
		if(comma != std::string::npos && comma+1 != s.size()) {
			this->value += std::stol(s.substr(comma+1, s.size()));
		}
	}

	
	//
	// div
	//
	
	template<typename T>
	Fixed div(T t, 
		typename std::enable_if<std::is_same<Fixed,T>::value >::type* = 0) 
		const noexcept
	{
		Fixed ret = *this;
		ret.value /= t.value;
		return ret;
	}

	template<typename T>
	Fixed div(T t, 
		typename std::enable_if<std::is_integral<T>::value >::type* = 0) 
		const noexcept
	{
		return this->div(Fixed(t));
	}

	template<typename T>
	Fixed div(T t, 
		typename std::enable_if<std::is_floating_point<T>::value >::type* = 0) 
		const noexcept
	{
		return this->div(Fixed(t));
	}
	
	//
	// multiply
	//
	template<typename T>
	Fixed multiply(T t, 
		typename std::enable_if<std::is_same<Fixed,T>::value >::type* = 0) 
		const noexcept
	{
		Fixed ret = *this;
		ret.value *= t.value;
		return ret;
	}

	template<typename T>
	Fixed multiply(T t, 
		typename std::enable_if<std::is_integral<T>::value >::type* = 0) 
		const noexcept
	{
		return this->multiply(Fixed(t));
	}

	template<typename T>
	Fixed multiply(T t, 
		typename std::enable_if<std::is_floating_point<T>::value >::type* = 0) 
		const noexcept
	{
		return this->multiply(Fixed(t));
	}

	//
	// add
	//
	template<typename T>
	Fixed add(T t, 
		typename std::enable_if<std::is_same<Fixed,T>::value >::type* = 0) 
		const noexcept
	{
		Fixed r;
		r.value = this->value + t.value;
		return r;
	}

	template<typename T>
	Fixed add(T t, 
		typename std::enable_if<std::is_integral<T>::value >::type* = 0) 
		const noexcept
	{
		return this->add(Fixed(t));
	}

	template<typename T>
	Fixed add(T t, 
		typename std::enable_if<std::is_floating_point<T>::value >::type* = 0) 
		const noexcept
	{
		return this->add(Fixed(t));
	}

	//
	// minus
	//
	template<typename T>
	Fixed minus(T t, 
		typename std::enable_if<std::is_same<Fixed,T>::value >::type* = 0) 
		const noexcept
	{
		Fixed r;
		r.value = this->value - t.value;
		return r;
	}

	template<typename T>
	Fixed minus(T t, 
		typename std::enable_if<std::is_integral<T>::value >::type* = 0) 
		const noexcept
	{
		return this->minus(Fixed(t));
	}

	template<typename T>
	Fixed minus(T t, 
		typename std::enable_if<std::is_floating_point<T>::value >::type* = 0) 
		const noexcept
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
		>::type* = 0) const noexcept
	{
		return this->value < t.value ? - 1 : this->value == t.value ? 0 : 1;
	}

	template<typename T>
	int compare(T t, 
		typename std::enable_if<std::is_integral<T>::value >::type* = 0) 
		const noexcept
	{
		return this->compare(Fixed(t));

	}

	template<typename T>
	int compare(T t, 
		typename std::enable_if<std::is_floating_point<T>::value >::type* = 0) 
		const noexcept
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

inline std::ostream& operator<<(std::ostream& os, const sweet::Fixed& d) {
	os<<(d.value >> 32u)<<'.'<<(d.value & sweet::Fixed::ClearTop);
	return os;
}
