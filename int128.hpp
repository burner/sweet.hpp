/* int128.hpp - a small 128 bit datatype consisting of one 64 bit integer
Author: Robert "burner" Schadek rburners@gmail.com License: LGPL 3 or higher
*/

#pragma once

#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <ostream>
#include <cstdint>
#include <limits>
#include <math.h>

// Assignment and Assignment-Conversion operators
// Infix addition

#define BITS62_0 0x7FFFFFFFFFFFFFFFLL
#define BIT_52	 0x0010000000000000LL
#define BITS51_0 0x000FFFFFFFFFFFFFLL
#define HIGHBIT	0x8000000000000000LL
#define ALLBITS	0xFFFFFFFFFFFFFFFFLL

#define SIGBITS_HI 0x7FFFFFFF00000000LL
#define HI_WORD 0xFFFFFFFF00000000LL
#define LO_WORD 0x00000000FFFFFFFFLL

struct int128;

int128 operator * ( const int128 & lhs, const int128 & rhs );
int128 operator * ( const int128 & lhs, uint64_t rhs );
int128 operator * ( const int128 & lhs, int32_t rhs );
int128 operator + ( const int128 & lhs, const int128 & rhs );
int128 operator + ( const int128 & lhs, uint64_t rhs );
int128 operator + ( const int128 & lhs, int32_t rhs );
int128 operator - ( const int128 & lhs, const int128 & rhs );
int128 operator - ( const int128 & lhs, uint64_t rhs );
int128 operator - ( const int128 & lhs, int32_t rhs );
int128 operator / ( const int128 & x, const int128 & d );
int128 operator / ( const int128 & x, uint64_t d );
int128 operator / ( const int128 & x, int32_t d );
int128 operator % ( const int128 & x, const int128 & d );
int128 operator % ( const int128 & x, uint64_t d );
int128 operator % ( const int128 & x, int64_t d );
int128 operator % ( const int128 & x, int32_t d );
int128 operator % ( const int128 & x, uint32_t d );
int128 operator += ( int128 & lhs, const int128 & rhs ) ;
int128 operator += ( int128 & lhs, const int64_t & rhs ) ;
int128 operator -= ( int128 & lhs, const int128 & rhs ) ;
int128 operator -= ( int128 & lhs, const int64_t & rhs ) ;
int128 operator *= ( int128 & lhs, const int128 & rhs ) ;
int128 operator *= ( int128 & lhs, const int64_t & rhs ) ;
int128 operator /= ( int128 & lhs, const int128 & rhs ) ;
int128 operator /= ( int128 & lhs, const int64_t & rhs ) ;
int operator < ( const int128 & lhs, const int128 & rhs );
inline int operator < ( const int128 & lhs, int32_t rhs );
inline int operator <= ( const int128 & lhs, const int128 & rhs );
inline int operator <= ( const int128 & lhs, int32_t rhs );
inline int operator == ( const int128 & lhs, const int128 & rhs );
inline int operator == ( const int128 & lhs, int32_t rhs );
inline int operator != ( const int128 & lhs, const int128 & rhs );
inline int operator != ( const int128 & lhs, int32_t rhs );
inline int operator > ( const int128 & lhs, const int128 & rhs );
inline int operator > ( const int128 & lhs, int32_t rhs );
inline int operator >= ( const int128 & lhs, const int128 & rhs );

struct int128 {
	int64_t high;
	uint64_t low;

	inline int128(): high(0), low(0) { }
	inline int128(int64_t h, uint64_t l) : high(h), low(l) {}
	inline int128(uint8_t x): high(0), low(x) { }
	inline int128(uint16_t x): high(0), low(x) { }
	inline int128(uint32_t x): high(0), low(x) { }
	inline int128(uint64_t x): high(0), low(x) { }
	//inline int128(const int128& x): high(x.high), low(x.low) { }

	inline int128(const std::string& str) : high(0), low(0) {
		int128 t;
		for(const auto& c : str) {
			t = t * static_cast<uint64_t>(10);
			t = t + static_cast<int32_t>(c + '0');
		}

		this->low = t.low;
		this->high = t.high;
	}

	template<typename T>
	void set(T t,
		typename std::enable_if<std::is_integral<T>::value >::type* = 0) 
	{
		this->low = t;
		this->high = 0;
	}

	template<typename T>
	int128 operator=(T t) {
		this->set(t);
		return *this;
	}

	explicit inline operator unsigned char() const {
		return this->low;
	}

	explicit inline operator char() const {
		return this->low | (this->high & HIGHBIT);
	}

	explicit inline operator unsigned short() const {
		return this->low;
	}

	explicit inline operator short() const {
		return this->low | (this->high & HIGHBIT);
	}

	explicit inline operator unsigned int() const {
		return this->low;
	}

	explicit inline operator int() const {
		int32_t result;

		result = (low & LO_WORD) | (high & HIGHBIT);
		return result;
	}

	explicit inline operator uint64_t() const {
		return low;
	}

	explicit inline operator int64_t() const {
		int64_t result;
		result = (low & BITS62_0) | (high & HIGHBIT);
		return result;
	}

	explicit inline operator double () const {
		double result;

		if(high >= 0) {
			result = static_cast<double>(high) * (static_cast<double>(18446744073709551616.0)) + 
				static_cast<double>(low);
		} else {
			int64_t h; 
		uint64_t l;
			h = high; l = low;

			h = ~h;
			l = ~l;
			l += 1;
			if(l == 0) {
				h += 1;
			}
			result = -static_cast<double>(h) * (static_cast<double>(18446744073709551616.0)) + 
				(static_cast<double>(l));
		}

		return result;
	}

	inline int128& operator++() {
		*this += 1;
		return *this;	
	}

	inline int128& operator--() {
		*this -= 1;
		return *this;	
	}

	inline int128(int8_t x) {
		this->low = x;
	
		if(x < 0) {
			this->high = -1;
		} else {
			this->high = 0;
		}
	}

	inline int128(int16_t x) {
		this->low = x;
	
		if(x < 0) {
			this->high = -1;
		} else {
			this->high = 0;
		}
	}

	inline int128(int32_t x) {
		this->low = x;
	
		if(x < 0) {
			this->high = -1;
		} else {
			this->high = 0;
		}
	}
	
	/*explicit int128(long long int x) {
		this->low = x;
	
		if(x < 0) {
			this->high = -1;
		} else {
			this->high = 0;
		}
	}*/

	inline int128(int64_t x) {
		this->low = x;
	
		if(x < 0) {
			this->high = -1;
		} else {
			this->high = 0;
		}
	}

	union DoubleUint64 {
		double d;
		uint64_t i;
	};
	
	inline int128(double x) {
		uint64_t t, m, h, l;
	
		if(x < -1.7014118346046e38) {
			// overflow negative
			this->high = HIGHBIT;
			this->low = 0;
		} else if(x < -9.2233720368547e18) {
			// take the 54 mantissa bits and shift into position
			//DoubleUint64 di;
			//di.d = x;
			//t = *(reinterpret_cast<uint64_t *>(&x));
			auto t2 = (reinterpret_cast<uint64_t *>(&x));
			t = *t2;
			m = (t & BITS51_0) | BIT_52;
			t = (t & BITS62_0) >> 52;
			//t = *(reinterpret_cast<uint64_t*>(reinterpret_cast<char*>(&x)));
			//t = di.i;
			// if x is 1.5 * 2^1, t will be 1024
			// if x is 1.5 * 2^52, t will be 1024+51 = 1075
			t = t - 1075;
			if(t > 64) {
				l = 0;
				h = m << (t-64);
		} else {
				l = m << t;
				h = m >> (64 - t);
			}
			this->low = ~l;
			this->high = ~h;
		} else if(x < 9.2233720368547e18) {
			// it will fit in a uint64_t
			this->low = static_cast<uint64_t>(x);
			this->high = ((x<0) ? -1 : 0);
		} else if(x < 1.7014118346046e38) {
			// take the 54 mantissa bits and shift into position
			//t = *(reinterpret_cast<uint64_t *>(&x));
			auto t2 = (reinterpret_cast<uint64_t *>(&x));
			t = *t2;
			//DoubleUint64 di;
			//di.d = x;
			//t = *(reinterpret_cast<uint64_t*>(reinterpret_cast<char*>(&x)));
			//t = di.i;
			m = (t & BITS51_0) | BIT_52;
			t = (t & BITS62_0) >> 52;
			// if x is 1.5 * 2^1, t will be 1024
			// if x is 1.5 * 2^52, t will be 1024+51 = 1075
			t = t - 1075;
			if(t > 64) {
				this->low = 0;
				this->high	= m << (t-64);
		} else {
				this->low = m << t;
				this->high	= m >> (64 - t);
			}
		} else {
			// overflow positive
			this->high = BITS62_0;
			this->low = ALLBITS;
		}
	}

	inline size_t digits() const {
		size_t l(log10(this->low));
		size_t h(log10(std::abs(this->high)));
		return l+h;
	}

	template<typename T>
	inline int128 pow(const T& t) const {
		bool pos(true);
		int128 r(*this);
		int128 ti(t);

		if(r < 0) {
			r = r * -1;
			pos = false;
		}

		while(ti > 1) {
			r *= *this;
			--ti;
		}

		if(!pos) {
			return r * -1;
		} else {
			return r;
		}
	}
};

inline int operator < ( const int128 & lhs, const int128 & rhs ) {
	if(lhs.high < rhs.high)
		return 1;
	if(rhs.high < lhs.high)
		return 0;
	// high components are equal
	if(lhs.low < rhs.low)
		return 1;
	return 0;
}

inline int operator < ( const int128 & lhs, int32_t rhs ) {
	int128 r(rhs);

	if(lhs.high < r.high)
		return 1;
	if(r.high < lhs.high)
		return 0;
	// high components are equal
	if(lhs.low < r.low)
		return 1;
	return 0;
}

inline int operator <= ( const int128 & lhs, const int128 & rhs ) {
	if((lhs.high == rhs.high) && (lhs.low == rhs.low))
		return 1;

	if(lhs.high < rhs.high)
		return 1;
	if(rhs.high < lhs.high)
		return 0;
	// high components are equal
	if(lhs.low < rhs.low)
		return 1;
	return 0;
}

inline int operator <= ( const int128 & lhs, int32_t rhs ) {
	int128 t(rhs);

	if((lhs.high == t.high) && (lhs.low == t.low))
		return 1;

	if(lhs.high < t.high)
		return 1;
	if(t.high < lhs.high)
		return 0;
	// high components are equal
	if(lhs.low < t.low)
		return 1;
	return 0;
}

inline int operator == ( const int128 & lhs, const int128 & rhs ) {
	if(lhs.high != rhs.high)
		return 0;
	if(lhs.low != rhs.low)
		return 0;
	return 1;
}

inline int operator == ( const int128 & lhs, int32_t rhs ) {
	int128 t(rhs);

	return ((lhs.high == t.high) && (lhs.low == t.low));
}

inline int operator != ( const int128 & lhs, const int128 & rhs ) {
	if(lhs.high != rhs.high)
		return 1;
	if(lhs.low != rhs.low)
		return 1;
	return 0;
}

inline int operator != ( const int128 & lhs, int32_t rhs ) {
	int128 t(rhs);

	return ( (lhs.high != t.high) || (lhs.low != t.low) );
}

inline int operator > ( const int128 & lhs, const int128 & rhs ) {
	if(lhs.high > rhs.high)
		return 1;
	if(rhs.high > lhs.high)
		return 0;
	// high components are equal
	if(lhs.low > rhs.low)
		return 1;
	return 0;
}

inline int operator > ( const int128 & lhs, int32_t rhs ) {
	int128 t(rhs);

	if(lhs.high > t.high)
		return 1;
	if(t.high > lhs.high)
		return 0;
	// high components are equal
	if(lhs.low > t.low)
		return 1;
	return 0;
}

inline int operator >= ( const int128 & lhs, const int128 & rhs ) {
	if((lhs.high == rhs.high) && (lhs.low == rhs.low))
		return 1;

	if(lhs.high > rhs.high)
		return 1;
	if(rhs.high > lhs.high)
		return 0;
	// high components are equal
	if(lhs.low > rhs.low)
		return 1;
	return 0;
}

inline int128 operator + ( const int128 & lhs, const int128 & rhs ) {
	int128 res;

	res.high = lhs.high + rhs.high;
	res.low = lhs.low + rhs.low;
	if(res.low < rhs.low) {
		(res.high)++;
	}

	return res;
}

inline int128 operator + ( const int128 & lhs, int64_t rhs ) {
	int128 res;

	if(rhs < 0) {
		res.high = lhs.high - 1;
		res.low = lhs.low + rhs;
		if(res.low < rhs) {
			(res.high)++;
		}
	} else {
		res.high = lhs.high;
		res.low = lhs.low + rhs;
		if(res.low < rhs) {
			(res.high)++;
		}
	}

	return res;
}

inline int128 operator + ( const int128 & lhs, uint64_t rhs ) {
	int128 res;

	res.high = lhs.high;
	res.low = lhs.low + rhs;
	if(res.low < rhs) {
		(res.high)++;
	}

	return res;
}

inline int128 operator + ( const int128 & lhs, int32_t rhs ) {
	int128 res;

	if(rhs < 0) {
		res.high = lhs.high - 1;
		res.low = lhs.low + rhs;
		if(res.low < rhs) {
			(res.high)++;
		}
	} else {
		res.high = lhs.high;
		res.low = lhs.low + rhs;
		if(res.low < rhs) {
			(res.high)++;
		}
	}

	return res;
}

inline int128 operator + ( int64_t lhs, const int128 & rhs ) {
	int128 res;
	int128 a;

	a.low = lhs;
	a.high = (lhs < 0) ? -1 : 0;

	res.high = a.high + rhs.high;
	res.low = a.low + rhs.low;
	if(res.low < rhs.low) {
		(res.high)++;
	}
	
	return res;
}

// Infix subtraction

inline int128 operator - ( const int128 & lhs, const int128 & rhs ) {
	int128 res;

	res.high = lhs.high - rhs.high;
	res.low = lhs.low - rhs.low;
	if(res.low > lhs.low) {
		// borrow
		(res.high)--;
	}
	
	return res;
}

inline int128 operator - ( const int128 & lhs, int32_t rhs ) {
	int128 res;
	int128 b;

	b.low = rhs;
	b.high = (rhs < 0) ? -1 : 0;

	res.high = lhs.high - b.high;
	res.low = lhs.low - b.low;
	if(res.low > lhs.low) {
		(res.high)--;
	}
	
	return res;
}

// Unary minus

inline int128 operator - ( const int128 & x ) {
	int128 res;

	res.high = ~(x.high);
	res.low = ~(x.low);
	res.low += 1;
	if(res.low == 0) {
		res.high += 1;
	}
	
	return res;
}

// Support routines for multiply, divide and modulo

static int128 int128_shr(int128 x) {
	int128 rv;

//	printf("%.16llX %016llX	>> ", x.high, x.low);
	rv.low = (x.low >> 1) | (x.high << 63);
	rv.high = x.high >> 1;

//	printf("%.16llX %016llX\n", rv.high, rv.low);

	return rv;
}

static int128 int128_shl(int128 x) {
	int128 rv;

	rv.high = (x.high << 1) | (x.low >> 63);
	rv.low = x.low << 1;

	return rv;
}

// Multiplication

static int128 mult1(int128 xi, int128 yi) {
	int128 rv2;
	uint64_t acc, ac2, carry, o1, o2;
	uint64_t a, b, c, d, e, f, g, h, ah, bh, ch, dh, bg, cg, dg, cf, df, de;

  /*            x      a  b  c  d
                y      e  f  g  h
                   ---------------
                      ah bh ch dh
                      bg cg dg
                      cf df
                      de
        --------------------------
                      -o2-- -o1--
                                  */

	d = xi.low & LO_WORD;
	c = (xi.low & HI_WORD) >> 32LL;
	b = xi.high & LO_WORD;
	a = (xi.high & HI_WORD) >> 32LL;

	h = yi.low & LO_WORD;
	g = (yi.low & HI_WORD) >> 32LL;
	f = yi.high & LO_WORD;
	e = (yi.high & HI_WORD) >> 32LL;
	
	acc = d * h;
	o1 = acc & LO_WORD;
	acc >>= 32LL;
	carry = 0;
	ac2 = acc + c * h; if(ac2 < acc) { carry++; }
	acc = ac2 + d * g; if(acc < ac2) { carry++; }
	rv2.low = o1 | (acc << 32LL);
	ac2 = (acc >> 32LL) | (carry << 32LL); carry = 0;

	acc = ac2 + b * h; if(acc < ac2) { carry++; }
	ac2 = acc + c * g; if(ac2 < acc) { carry++; }
	acc = ac2 + d * f; if(acc < ac2) { carry++; }
	o2 = acc & LO_WORD;
	ac2 = (acc >> 32LL) | (carry << 32LL);

	acc = ac2 + a * h;
	ac2 = acc + b * g;
	acc = ac2 + c * f;
	ac2 = acc + d * e;
	rv2.high = (ac2 << 32LL) | o2;

	return rv2;
}

inline int128 operator * ( const int128 & lhs, const int128 & rhs ) {
	int128 rv;

	rv = mult1(lhs, rhs);

	return rv;
}

inline int128 operator * ( const int128 & lhs, uint64_t rhs ) {
	int128 t;
	int128 rv;

	t = rhs;
	rv = mult1(lhs, int128(rhs));

	return rv;
}

inline int128 operator * ( const int128 & lhs, int32_t rhs ) {
	int128 t;
	int128 rv;

	t = rhs;
	rv = mult1(lhs, int128(rhs));

	return rv;
}

// Division

static int128 div1(int128 x, int128 d, int128 *r) {
	int s;
	int128 d1, p2, rv;

//printf("divide %.16llX %016llX / %.16llX %016llX\n", x.high, x.low, d.high, d.low);

	/* check for divide by zero */
	if((d.low == 0) && (d.high == 0)) {
		rv.low = x.low / d.low; /* This will cause runtime error */
	}

	s = 1;
	if(x < 0) {
		// notice that MININT will be unchanged, this is used below.
		s = - s;
		x = - x;
	}
	if(d < 0) {
		s = - s;
		d = - d;
	}

	if(d == 1) {
		/* This includes the overflow case MININT/-1 */
		rv = x;
		x = 0;
	//} else if(x < ((int128) d)) {
	} else if(x < d) {
		/* x < d, so quotient is 0 and x is remainder */
		rv = 0;
	} else {
		rv = 0;

		/* calculate biggest power of 2 times d that's <= x */
		p2 = 1; d1 = d;
		x = x - d1;
		while(x >= d1) {
			x = x - d1;
			d1 = d1 + d1;
			p2 = p2 + p2;
		}
		x = x + d1;

		while(p2 != 0) {
//printf("x %.16llX %016llX d1 %.16llX %016llX\n", x.high, x.low, d1.high, d1.low);
			if(x >= d1) {
				x = x - d1;
				rv = rv + p2;
//printf("`.. %.16llX %016llX\n", rv.high, rv.low);
			}
			p2 = int128_shr(p2);
			d1 = int128_shr(d1);
		}

		/* whatever is left in x is the remainder */
	}

	/* Put sign in result */
	if(s < 0) {
		rv = - rv;
	}

	/* return remainder if they asked for it */
	if(r) {
		*r = x;
	}

	return rv;
}

inline int128 operator / ( const int128 & x, const int128 & d ) {
	int128 rv;

	rv = div1(x, d, 0);

	return rv;
}

inline int128 operator / ( const int128 & x, uint64_t d ) {
	int128 t;
	int128 rv;

	t.high = 0; t.low = d;
	rv = div1(x, t, 0);

	return rv;
}

// Modulo operator

inline int128 operator % ( const int128 & x, const int128 & d) {
	int128 rv;

	//rv = rv - (rv / ( d * d));
	rv = x - (d * (x / d));

	return rv;
}

inline int128 operator % ( const int128 & x, uint64_t d ) {
	return x % int128(d);
}

inline int128 operator % ( const int128 & x, int32_t d ) {
	return x % int128(d);
}

inline int128 operator % ( const int128 & x, int64_t d ) {
	return x % int128(d);
}

inline int128 operator % ( const int128 & x, uint32_t d ) {
	return x % int128(d);
}

// Comparison operators


// Conversion for output

static uint64_t int128_u64(int128 x) {
	return(x.low);
}

static uint32_t int128_uint32_t(int128 x) {
	uint32_t rv;

	rv = x.low;
	return(rv);
}

static void int128_str(int128 x, char *s) {
	int d, nd, going;
	int128 t, p10;

	//if(x < ((int128) 0)) {
	if(x < 0) {
		*s = '-'; s++;
		x = - x;
	}
	//if(x == ((int128) 0)) {
	if(x == 0) {
		*s = '0'; s++;
		*s = 0; return;
	}

	// Count number of digits in x
	p10 = 1; nd = 0; going = 1;
	while((p10 <= x) && (going)) {
		//p10 = p10 + p10; if(p10 < ((int128) 0)) { going = 0; }
		p10 = p10 + p10; if(p10 < 0) { going = 0; }
		t = p10;
		//p10 = p10 + p10; if(p10 < ((int128) 0)) { going = 0; }
		//p10 = p10 + p10; if(p10 < ((int128) 0)) { going = 0; }
		//p10 = p10 + t; if(p10 < ((int128) 0)) { going = 0; }
		p10 = p10 + p10; if(p10 < 0) { going = 0; }
		p10 = p10 + p10; if(p10 < 0) { going = 0; }
		p10 = p10 + t; if(p10 < 0) { going = 0; }
		nd++;
	}

	// Extract each digit
	while(nd > 0) {
		int i;

		nd--;
		p10 = 1;
		for(i=0; i<nd; i++) {
			p10 = p10 + p10;
			t = p10;
			p10 = p10 + p10;
			p10 = p10 + p10;
			p10 = p10 + t;
		}
		d = 0;
		while(x >= p10) {
			x = x - p10;
			d++;
		}
		*s=('0' + d); s++;
	}
	*s=0;
}

inline std::ostream& operator<<(std::ostream& os, int128 i) {
	char out[128] = {'\0'};
	int128_str(i, out);
	os<<out;
	return os;
}

inline int128 operator += ( int128 & lhs, const int128 & rhs )  {
	lhs = lhs + rhs;
	return lhs;
}

inline int128 operator += ( int128 & lhs, const int64_t & rhs )  {
	lhs = lhs + rhs;
	return lhs;
}

inline int128 operator -= ( int128 & lhs, const int128 & rhs )  {
	lhs = lhs - rhs;
	return lhs;
}

inline int128 operator -= ( int128 & lhs, const int64_t & rhs )  {
	lhs = lhs - int128(rhs);
	return lhs;
}

inline int128 operator *= ( int128 & lhs, const int128 & rhs )  {
	lhs = lhs * rhs;
	return lhs;
}

inline int128 operator *= ( int128 & lhs, const int64_t & rhs )  {
	lhs = lhs * int128(rhs);
	return lhs;
}

inline int128 operator /= ( int128 & lhs, const int128 & rhs )  {
	lhs = lhs / rhs;
	return lhs;
}

inline int128 operator /= ( int128 & lhs, const int64_t & rhs )  {
	lhs = lhs / int128(rhs);
	return lhs;
}

template<typename T>
struct is_sweet_int128 : std::false_type {};

template<>
struct is_sweet_int128<int128> : std::true_type {};

namespace std { 

template<>        // define your specialization
class numeric_limits<int128> {
  public:
	static unsigned long long max() {
		return std::numeric_limits<uint64_t>::max();
	}

	static long long min() {
		return std::numeric_limits<int64_t>::max();
	}
};

}
