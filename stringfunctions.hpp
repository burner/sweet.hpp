/* stringfunctions.hpp - a small header containing everyday string funcitons.
Author: Robert "burner" Schadek rburners@gmail.com License: LGPL 3 or higher
*/

#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <emmintrin.h>
#include <immintrin.h>

#include <conv.hpp>

namespace sweet {

template<typename V>
V& split(const std::string& s, char delim, V& elems) {
    std::stringstream ss(s);
    std::string item;
	std::back_insert_iterator<V> inserter(elems);

    while(std::getline(ss, item, delim)) {
		*inserter = to<typename V::value_type>(item);
		++inserter;
    }
    return elems;
}


template<typename V>
V split(const std::string& s, char delim = ',') {
    V elems;
    split(s, delim, elems);
    return std::move(elems);
}

// from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}

}

namespace sweet {

inline bool stringCmpNaive(const std::string& a, const std::string& b) {
	const size_t aLen = a.size();
	const size_t bLen = b.size();

	if(aLen != bLen) {
		return false;
	}

	for(size_t i = 0; i < aLen; ++i) {
		if(a[i] != b[i]) {
			return false;
		}
	}

	return true;
}

inline bool stringCmpSSE(const std::string& a, const std::string& b) {
	const size_t aLen = a.size();
	const size_t bLen = b.size();

	if(aLen != bLen) {
		return false;
	}

	// _mm_cmpeq_epi8
	// _mm_lddqu_si128
	//
	size_t i = 0;

#ifdef __SSE3__
	const char* aPtr = a.c_str();
	const char* bPtr = b.c_str();

	for(; aLen - i > 16; i += 16u) {
		__m128i a16 = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(aPtr));
		__m128i b16 = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(bPtr));
		__m128i rslt = _mm_cmpeq_epi8(a16, b16);
		const auto vmask = _mm_movemask_epi8(rslt);
		if(vmask != 0xFFFF) {
			return false;
		}

		aPtr += 16;
		bPtr += 16;
	}
#endif

	for(; i < aLen; ++i) {
		if(a[i] != b[i]) {
			return false;
		}
	}

	return true;
}

}
