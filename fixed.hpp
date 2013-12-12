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

#include <logger.hpp>

namespace sweet {

class Fixed {
	//friend std::ostream& operator<<(std::ostream&,const sweet::Fixed&);
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

private:

	//
	// setting
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
		this->value = t << Shift;
	}

	template<typename T>
	void set(T t,
		typename std::enable_if<std::is_floating_point<T>::value >::type* = 0) 
	{

		int64_t h = static_cast<int64_t>(t);
		this->value = h << Fixed::Shift;
		double l = fmod(t, 1.0);
		l *= Fixed::ShiftValue;
		int64_t li = static_cast<int64_t>(l);
		this->value += li;
	}

	template<typename T>
	void set(T s,
		typename std::enable_if<
			std::is_same<typename std::remove_reference<T>::type,std::string>::value
		>::type* = 0) 
	{
		this->setString(s);
	}

	template<typename T>
	void set(T s,
		typename std::enable_if<
			std::is_same<typename std::remove_reference<T>::type,const char*>::value
		>::type* = 0) 
	{
		this->setString(s);
	}

	void setString(const std::string& s) {
		auto comma = s.find(',');
		this->value = std::stol(s.substr(0, comma))<<Fixed::Shift;
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
	int64_t value;
	static const int Shift = 32;
	static const int64_t ShiftValue = (1llu<<Shift);
};

}

std::ostream& operator<<(std::ostream& os, const sweet::Fixed& d) {
	os<<(d.value>>sweet::Fixed::Shift)<<"."<<(d.value & (sweet::Fixed::ShiftValue-1));
	//os<<(d.value)<<"."<<(d.value&1023);
	return os;
}
