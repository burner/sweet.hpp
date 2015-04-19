// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
#ifndef SWEETHPP_BITVECTOR
#define SWEETHPP_BITVECTOR

#include <vector>
#include <iostream>

class BitVector {
public:
	unsigned int threeTwo;
	std::vector<bool> rest;

public:
	static bool _bittest(unsigned int base, unsigned int offset) {
		return base & (1u << offset);
    }

	inline bool operator[](const size_t idx) const {
		if(idx < 32) {
			return _bittest(threeTwo, idx);
		} else {
			const size_t nIdx = idx-32;
			if(nIdx >= rest.size()) {
				return false;
			}
			return rest[nIdx];
		}
	}

	inline void set(const size_t idx) {
		if(idx < 32u) {
			threeTwo |= (1u<<idx);
		} else {
			const size_t nIdx = idx-32u;
			if(nIdx >= rest.size()) {
				rest.resize(nIdx+1, false);
			}
			rest[nIdx] = true;
		}
	}
	
	inline void unset(const size_t idx) {
		if(idx < 32u) {
			threeTwo &= ~(1u<<idx);
		} else {
			const size_t nIdx = idx-32u;
			if(nIdx >= rest.size()) {
				rest.resize(nIdx+1u, false);
			}
			rest[nIdx-32u] = false;
		}
	}

public:
	inline BitVector(unsigned int b = 0) : threeTwo(b) {}
};

#endif
