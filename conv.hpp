// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
#pragma once

#include <string>
#include <limits>
#include <assert.h>
#include <type_traits>
#include <stdexcept>

#include <int128.hpp>
#include <format.hpp>
#include <compare.hpp>

template<typename T, typename F>
bool convIsOk(const F f, typename std::enable_if<
			std::is_signed<T>::value && std::is_signed<F>::value
		>::type* = 0) {
	return std::numeric_limits<T>::min() <= f && 
		std::numeric_limits<T>::max() >= f;
}

template<typename T, typename F>
bool convIsOk(const F f, typename std::enable_if<
			std::is_unsigned<T>::value && std::is_signed<F>::value
		>::type* = 0) {
	return f >= 0 
		&& static_cast<uint64_t>(f) <= std::numeric_limits<T>::max();
}

template<typename T, typename F>
bool convIsOk(const F f, typename std::enable_if<
			std::is_signed<T>::value && std::is_unsigned<F>::value
		>::type* = 0) {
	return f <= std::numeric_limits<T>::max();
}

template<typename T, typename F>
bool convIsOk(const F f, typename std::enable_if<
			std::is_unsigned<T>::value && std::is_unsigned<F>::value
		>::type* = 0) {
	return f <= std::numeric_limits<T>::max();
}

template<typename T, typename F>
bool convIsOk(const F f, typename std::enable_if<
		is_sweet_int128<F>::value>::type* = 0)
{
	return sweet::greaterEqual(f, std::numeric_limits<T>::min()) &&
		sweet::lessEqual(f, std::numeric_limits<T>::max());
}

template<typename T>
class NumberConv {
public:
	template<typename F>
	T operator()(const F f, 
			typename std::enable_if<std::is_fundamental<F>::value, F>
			::type* = 0) const 
	{
		if(convIsOk<T,F>(f)) {
			return static_cast<T>(f);
		} else {
			throw std::range_error(format("illegal conv %d <= %d <= %d", 
				std::numeric_limits<T>::min(), f, std::numeric_limits<T>::max()));
		}
	}
};

template<typename S>
class ConvStruct {
public:
	S operator()(const std::string& s) {
		return static_cast<S>(stoi(s));
	}
};

template<>
class ConvStruct<char> {
public:
	char operator()(const std::string& s) {
		return static_cast<char>(stoi(s));
	}
};

template<>
class ConvStruct<unsigned char> {
public:
	unsigned char operator()(const std::string& s) {
		unsigned long tmp = stoul(s);
		assert(tmp <= std::numeric_limits<unsigned char>::max());
		return static_cast<unsigned char>(tmp);
	}
};

template<>
class ConvStruct<short> {
public:
	short operator()(const std::string& s) {
		return static_cast<short>(stoi(s));
	}
};

template<>
class ConvStruct<unsigned short> {
public:
	unsigned short operator()(const std::string& s) {
		unsigned long tmp = stoul(s);
		assert(tmp <= std::numeric_limits<unsigned short>::max());
		return static_cast<unsigned short>(tmp);
	}
};

template<>
class ConvStruct<int> {
public:
	int operator()(const std::string& s) {
		return stoi(s);
	}
};

template<>
class ConvStruct<unsigned int> {
public:
	unsigned int operator()(const std::string& s) {
		unsigned long tmp = stoul(s);
		assert(tmp <= std::numeric_limits<unsigned int>::max());
		return static_cast<unsigned int>(tmp);
	}
};

template<>
class ConvStruct<bool> {
public:
	bool operator()(const std::string& s) {
		unsigned long tmp = stoul(s);
		assert(tmp <= std::numeric_limits<unsigned int>::max());
		return s == "true";
	}
};

template<>
class ConvStruct<long> {
public:
	long operator()(const std::string& s) {
		return stol(s);
	}
};

template<>
class ConvStruct<unsigned long> {
public:
	unsigned long operator()(const std::string& s) {
		return stoul(s);
	}
};

template<>
class ConvStruct<long long> {
public:
	long long operator()(const std::string& s) {
		return stoll(s);
	}
};

template<>
class ConvStruct<unsigned long long> {
public:
	unsigned long long operator()(const std::string& s) {
		return stoull(s);
	}
};

template<>
class ConvStruct<float> {
public:
	float operator()(const std::string& s) {
		return stof(s);
	}
};

template<>
class ConvStruct<double> {
public:
	double operator()(const std::string& s) {
		return stod(s);
	}
};

template<>
class ConvStruct<long double> {
public:
	long double operator()(const std::string& s) {
		return stold(s);
	}
};

template<typename T>
struct is_string {
	enum { value = std::is_same<std::string, T>::value || std::is_same<const char*, T>::value ||
		(std::is_array<T>::value && std::is_same<char, typename std::remove_extent<T>::type>::value) };
};


template<typename S, typename T>
typename std::enable_if<is_string<T>::value && is_string<S>::value, S>::type
to(const T& in) {
	return in;
}

template<typename S, typename T>
typename std::enable_if<!is_string<T>::value && is_string<S>::value, S>::type
to(const T& in) {
	return std::to_string(in);
}

template<typename S, typename T>
typename std::enable_if<is_string<T>::value && !is_string<S>::value, S>::type
to(const T& in) {
	ConvStruct<S> ret;
	return ret(in);
}

template<typename S, typename T>
typename std::enable_if<!is_string<T>::value && !is_string<S>::value, S>::type
to(const T& in) {
	if(convIsOk<S>(in)) {
		return static_cast<S>(in);
	} else {
		throw std::range_error(format("illegal conv %d <= %d <= %d", 
			std::numeric_limits<T>::min(), in, std::numeric_limits<T>::max()));
	}
}
