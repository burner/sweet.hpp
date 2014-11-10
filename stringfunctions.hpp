/* stringfunctions.hpp - a small header containing everyday string funcitons.
Author: Robert "burner" Schadek rburners@gmail.com License: LGPL 3 or higher
*/

#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

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
