// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
#pragma once

#include <stdint.h>
#include <type_traits>
#include <limits>
#include <stdexcept>
#include <string>

namespace sweet {

static size_t bits_in_uint8[256] = {
0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

template<typename Store>
struct Count {
	inline static size_t count(const Store) {
		return 0;
	}
};

template<>
struct Count<uint8_t> {
	inline static size_t count(const uint8_t n) {
		return bits_in_uint8[n];
	}
};

template<>
struct Count<uint16_t> {
	inline static size_t count(const uint16_t n) {
		return bits_in_uint8[n & 0xFFU] + bits_in_uint8[(n >> 8) & 0xFFU];
	}
};

template<>
struct Count<uint32_t> {
	inline static size_t count(const uint32_t n) {
		return bits_in_uint8[n & 0xFFU] + bits_in_uint8[(n >> 8) & 0xFFU] + 
			bits_in_uint8[(n >> 16) & 0xFFU] + bits_in_uint8[(n >> 24) & 0xFFU];
	}
};

template<>
struct Count<uint64_t> {
	inline static size_t count(const uint64_t n) {
		return bits_in_uint8[n & 0xFFU] + bits_in_uint8[(n >> 8) & 0xFFU] + 
			bits_in_uint8[(n >> 16) & 0xFFU] + bits_in_uint8[(n >> 24) & 0xFFU] +
			bits_in_uint8[(n >> 32) & 0xFFU] + bits_in_uint8[(n >> 40) & 0xFFU] +
			bits_in_uint8[(n >> 48) & 0xFFU] + bits_in_uint8[(n >> 56) & 0xFFU];
	}
};

template<typename Store>
struct Bitset {
	struct Reference {
		Bitset<Store>& ref;
		size_t idx;

		inline Reference(Bitset<Store>& r, const size_t i) : ref(r), idx(i) {
		}

		inline Reference(const Reference& r) : ref(r.ref), idx(r.idx) {
		}

		inline operator bool() const {
			return const_cast<const Bitset<Store>&>(this->ref)[idx];
		}

		inline bool operator~() const {
			return !const_cast<const Bitset<Store>&>(this->ref)[idx];
		}

		inline Reference& operator=(const bool v) {
			this->ref.set(this->idx, v);
			return *this;
		}

		inline Reference& operator=(const Reference& r) {
			this->ref = r.ref;
			this->idx = r.idx;
			return *this;
		}
	};

	Store store;

	// constructor
	
	inline explicit Bitset(const Store defaultValue = 0) : store(defaultValue) {
	}

	inline bool operator[](const size_t idx) const {
		return store & (1<<idx);
	}

	inline Reference operator[](const size_t idx) {
		return Reference(*this, idx);
	}

	// access
	
	inline bool test(const size_t idx) const {
		if(idx > (sizeof(Store) * 8)) {
			throw std::range_error("out of bound access to Bitset");
		} else {
			return (*this)[idx];
		}
	}

	constexpr size_t size() const {
		return sizeof(Store) * 8;
	}

	inline size_t count() const {
		return Count<Store>::count(this->store);	
	}

	// modify
	
	inline Bitset<Store>& set() {
		this->store = std::numeric_limits<Store>::max();

		return *this;
	}

	inline Bitset<Store>& set(const size_t idx, const bool value = true) {
		/*if(idx > (sizeof(Store) * 8)) { SPEEEEEEEEEEEEED
			throw std::range_error("out of bound access to Bitset");
		} else {*/
			this->store ^= (-value ^ this->store) & (1 << idx);
		//}

		return *this;
	}
	
	inline Bitset<Store>& reset() {
		this->store = 0;

		return *this;
	}

	inline Bitset<Store>& reset(const size_t idx) {
		if(idx > (sizeof(Store) * 8)) {
			throw std::range_error("out of bound access to Bitset");
		} else {
			this->store &= ~(1<<idx);
		}

		return *this;
	}
	
	inline Bitset<Store>& flip() {
		this->store = ~this->store;

		return *this;
	}

	inline Bitset<Store>& flip(const size_t idx) {
		if(idx > (sizeof(Store) * 8)) {
			throw std::range_error("out of bound access to Bitset");
		} else {
			this->store ^= (1<<idx);
		}

		return *this;
	}

	inline Bitset<Store>& operator&=(const Bitset<Store>& a) {
		this->store &= a.store;
		return *this;
	}

	inline Bitset<Store>& operator|=(const Bitset<Store>& a) {
		this->store |= a.store;
		return *this;
	}

	inline Bitset<Store>& operator^=(const Bitset<Store>& a) {
		this->store ^= a.store;
		return *this;
	}

	inline Bitset<Store> operator~() {
		return Bitset<Store>(~this->store);
	}

	inline Bitset<Store> operator<<(const size_t idx) const {
		return Bitset<Store>(this->store << idx);
	}

	inline Bitset<Store> operator>>(const size_t idx) const {
		return Bitset<Store>(this->store >> idx);
	}

	inline Bitset<Store> operator<<(const size_t idx) {
		this->store <<= idx;
		return *this;
	}

	inline Bitset<Store> operator>>(const size_t idx) {
		this->store >>= idx;
		return *this;
	}

	// compare
	
	inline bool operator==(const Bitset<Store>& rhs) const {
		return this->store == rhs.store;
	}

	inline bool operator!=(const Bitset<Store>& rhs) const {
		return this->store != rhs.store;
	}

	inline bool all() const {
		return this->store == std::numeric_limits<Store>::max();
	}

	inline bool any() const {
		return this->store != 0;
	}
	
	inline bool none() const {
		return this->store == 0;
	}

	// conversion
	
	inline std::string to_string() const {
		std::string ret(this->size(), ' ');
		for(size_t i = 0; i < this->size(); ++i) {
			ret[i] = this->test(i) ? '1' : '0';	
		}

		return ret;
	}

	inline unsigned long to_ulong() const {
		return static_cast<unsigned long>(this->store);
	}

	inline unsigned long to_ullong() const {
		return static_cast<unsigned long long>(this->store);
	}
};

template<typename Store>
Bitset<Store> operator&(const Bitset<Store>& a, const Bitset<Store>& b) {
	return Bitset<Store>(a.store & b.store);
}

template<typename Store>
Bitset<Store> operator|(const Bitset<Store>& a, const Bitset<Store>& b) {
	return Bitset<Store>(a.store | b.store);
}

template<typename Store>
Bitset<Store> operator^(const Bitset<Store>& a, const Bitset<Store>& b) {
	return Bitset<Store>(a.store ^ b.store);
}
}

namespace std {
	template<typename T> struct hash<sweet::Bitset<T>> {
		size_t operator()(const sweet::Bitset<T>& x) const {
			return x.store;
		}
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const sweet::Bitset<T>& x) {
		out<<x.to_string();
		return out;
	}	
}
