#pragma once
#include <cstddef>
#include <stdexcept>

namespace sweet {
	template<typename T, size_t Capacity>
	class Fector {
	public:
		typedef T value_type;
		typedef size_t size_type;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T* pointer;
		typedef const T* const_pointer;

	private:
		T data[Capacity];
		long idx; 

	public:
		// constructor
		Fector() : idx(-1) {
		}

		//access
		
		T& operator[](const size_t i) {
			return this->data[i];
		}

		const T& operator[](const size_t i) const {
			return this->data[i];
		}

		T& at(const size_t i) {
			if(i < this->idx) {
				return this->data[i];
			} else {
				throw std::out_of_range("at index out of range");
			}
		}

		const T& at(const size_t i) const {
			if(i < this->idx) {
				return this->data[i];
			} else {
				throw std::out_of_range("at index out of range");
			}
		}

		T& front() {
			return this->data[0u];
		}

		const T& front() const {
			return this->data[0u];
		}

		T& back() {
			return this->data[idx];
		}

		const T& back() const {
			return this->data[idx];
		}

		// capacity

		bool empty() const {
			return this->idx == -1;
		}

		bool size() const {
			return this->idx+1;
		}

		constexpr size_t max_size() const {
			return Capacity;
		}

		constexpr size_t capacity() const {
			return Capacity;
		}

		// modifier

		void clear() {
			this->idx = -1;
		}

		void push_back(const T& value) {
			if(idx+1 <= static_cast<long>(Capacity)) {
				this->data[++idx] = value;
			} else {
				throw std::out_of_range("fector full");
			}
		}

		void push_back(T&& value) {
			if(idx+1 <= static_cast<long>(Capacity)) {
				this->data[++idx] = value;
			} else {
				throw std::out_of_range("fector full");
			}
		}

		void pop_back() {
			--this->idx;
		}

		template<typename... Args>
		void emplace_back(Args&&... args) {
			if(idx+1 <= static_cast<long>(Capacity)) {
				new(this->data[++idx]) T(args...);
			} else {
				throw std::out_of_range("fector full");
			}
			
		}
	};
}
