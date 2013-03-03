// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
#ifndef SWEET_HPP_CONV
#define SWEET_HPP_CONV
#include <string>
#include <limits>
#include <assert.h>
#include <type_traits>

template<typename T>
std::string to(T in) {
	return std::to_string(in);
}

template<typename S>
class ConvStruct {
public:
	S operator()(const std::string& s) {
		//assert(false);
		return static_cast<S>(s);
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

template<typename S>
S to(const std::string& in) {
	ConvStruct<S> ret;
	return ret(in);
}

/*
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
}*/
#endif
