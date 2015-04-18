// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
#pragma once

#include <algorithm>
#include <functional>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iterator>
#include <string>
#include <memory>

#include "sweetqlimpl/types.hpp"
#include "sweetqlimpl/sqliteimpl.hpp"

template<typename Impl>
class SweetQL {
public:

	inline SweetQL(Impl& i) : impl(i) {

	}

	inline void beginTransaction() {
		this->impl.beginTransaction();
	}

	inline void endTransaction() {
		this->impl.endTransaction();
	}

	template<typename S>
	bool insert(S& t) {
		return this->impl. template insert<S>(t);
	}

	template<typename S, typename It>
	bool insert(It be, It en) {
		return this->impl. template insert<S>(be, en);
	}

	template<typename S>
	void createTable() {
		this->impl. template createTable<S>();
	}

	template<typename S>
	void remove(S& s) {
		this->impl.remove(s);
	}

	template<typename S>
	std::pair<typename Impl:: template Iterator<S>, 
		typename Impl:: template Iterator<S>> select(const std::string& where = "")
	{
		return this->impl. template select<S>(where);
	}

	template<typename S>
	S selectOne(bool& wasSet, const std::string& where = "") {
		auto its = this->select<S>(where);
		if(its.first == its.second) {
			wasSet = false;
			return S();
		}

		wasSet = true;
		return *its.first;
	}

	template<typename S, typename T, typename R>
	std::pair<typename Impl:: template Iterator<S>, typename Impl:: template Iterator<S>> join(
			const std::string& where = "") 
	{
		return this->impl. template join<S,T,R>(where);
	}

private:

	Impl& impl;
};
