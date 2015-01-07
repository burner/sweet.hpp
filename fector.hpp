#pragma once
#include <cstddef>
#include <stdexcept>
#include <iostream>
#include <new>
#include <format.hpp>

namespace sweet {
	template<typename T, size_t Capacity = 128>
	class Fector {
	public:
		typedef T value_type;
		typedef size_t size_type;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T* iterator;
		typedef const T* const_iterator;

	private:
		T data[Capacity+1];
		size_t idx; 

	public:
		// constructor
		inline Fector() : idx(0) {
		}

		/*
		inline Fector(const Fector& other) : idx(0) {
			size_t otherSize = other.size();
			for(size_t i = 0; i < otherSize; ++i) {
				this->push_back(other[i]);
			}
		}

		inline Fector(Fector&& other) : idx(0) {
			size_t otherSize = other.size();
			for(size_t i = 0; i < otherSize; ++i) {
				this->push_back(other[i]);
			}
		}*/

		inline Fector(const size_t cnt, const T& value) : Fector() {
			this->resize(cnt, value);
		}

		inline void resize(const size_t cnt, const T& value) {
			while(cnt < this->idx) {
				this->pop_back();
			}

			while(cnt > this->idx) {
				this->push_back(value);
			}
		}

		//access
		
		inline T& operator[](const size_t i) {
			return this->data[i];
		}

		inline const T& operator[](const size_t i) const {
			return this->data[i];
		}

		inline T& at(const size_t i) {
			if(i < this->idx) {
				return this->data[i];
			} else {
				throw std::out_of_range(format("at index(%u) out of range size(%u)",
					i, this->size())
				);
			}
		}

		inline const T& at(const size_t i) const {
			if(i < this->idx) {
				return this->data[i];
			} else {
				throw std::out_of_range(format("at index(%u) out of range size(%u)",
					i, this->size())
				);
			}
		}

		inline T& front() {
			return this->data[0u];
		}

		inline const T& front() const {
			return this->data[0u];
		}

		inline T& back() {
			return this->data[idx-1];
		}

		inline const T& back() const {
			return this->data[idx-1];
		}

		// capacity

		inline bool empty() const {
			return this->idx == 0;
		}

		inline size_t size() const {
			return this->idx;
		}

		inline constexpr size_t max_size() const {
			return Capacity;
		}

		inline constexpr size_t capacity() const {
			return Capacity;
		}

		// modifier

		inline void clear() {
			this->idx = 0;
		}

		inline void push_back(const T& value) {
			if(idx <= Capacity) {
				this->data[idx] = value;
				++idx;
			} else {
				throw std::out_of_range("fector full can not push_back");
			}
		}

		inline void push_back(T&& value) {
			if(idx <= Capacity) {
				this->data[idx] = value;
				++idx;
			} else {
				throw std::out_of_range("fector full can not push_back(&&)");
			}
		}

		template<typename... Args>
		inline void emplace(Args... args) {
			if(idx <= Capacity) {
				new (&this->data[idx]) T(args...);
				++idx;
			} else {
				throw std::out_of_range("fector full can not push_back(&&)");
			}
		}

		inline void pop_back() {
			this->data[this->idx - 1].~T();
			--this->idx;
		}

		inline iterator insert(iterator it, const T& value) {
			if(idx <= Capacity) {
				auto en(this->end());
				do {
					auto enm = en;
					--enm;
					*en = *enm;
					--en;
				} while(en!= it);
				*it = value;
				++this->idx;
				return it;
			} else {
				throw std::out_of_range("fector full can not insert");
			}
		}

		inline iterator erase(iterator pos) {
			const_iterator e = this->end();
			auto ret = pos;
			auto next = pos + 1u;
			while(next < e) {
				*pos = *next;
				++pos;
				++next;
			}

			this->pop_back();

			return ret;
		}

		// iterator
		
		iterator begin() {
			return &this->data[0];
		}

		iterator end() {
			return &this->data[this->idx];
		}

		const_iterator begin() const {
			return &this->data[0];
		}

		const_iterator end() const {
			return &this->data[this->idx];
		}

		iterator rbegin() {
			return &this->data[this->idx-1u];
		}

		iterator rend() {
			return &this->data[0];
		}

		const_iterator rbegin() const {
			return &this->data[this->idx-1u];
		}

		const_iterator rend() const {
			return &this->data[0];
		}
	};
}

template<typename>
struct is_sweet_Fector : std::false_type {};

template<typename T, std::size_t N>
struct is_sweet_Fector<sweet::Fector<T,N>> : std::true_type {};

template<typename T, size_t S>
std::ostream& operator<<(std::ostream& out, const sweet::Fector<T,S>& v) {
	for(typename sweet::Fector<T,S>::const_pointer it = v.begin(); 
			it != v.end(); ++it) 
	{
		typename sweet::Fector<T,S>::const_reference tmp = *it; 
		if(is_sweet_Fector<typename sweet::Fector<T,S>::value_type>::value) {
			format(out, "%s", tmp);
		} else {
			format(out, "%s ", tmp);
		}
	}
	out<<std::endl;

	return out;
}
