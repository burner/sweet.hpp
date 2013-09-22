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
	/*
		int old = 0;
		 __asm__ __volatile__("btl %2,%1\n\tsbbl %0,%0 "
		:"=r" (old)
		:"m" (*Base), "Ir" (Offset));
		return (bool)(old!=0);
	*/
		return base & (1 << offset);
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
		if(idx < 32) {
			threeTwo |= (1<<idx);
		} else {
			const size_t nIdx = idx-32;
			if(nIdx >= rest.size()) {
				rest.resize(nIdx+1, false);
			}
			rest[nIdx] = true;
		}
	}
	
	inline void unset(const size_t idx) {
		if(idx < 32) {
			threeTwo &= ~(1<<idx);
		} else {
			const size_t nIdx = idx-32;
			if(nIdx >= rest.size()) {
				rest.resize(nIdx+1, false);
			}
			rest[nIdx-32] = false;
		}
	}

public:
	inline BitVector(int b = 0) : threeTwo(b) {}
};

#endif
