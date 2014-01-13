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
V split(const std::string& s, char delim) {
    V elems;
    split(s, delim, elems);
    return std::move(elems);
}

}
