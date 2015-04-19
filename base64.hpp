// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
#pragma once

#include <string>
#include <istream>
#include <ostream>


static const char kBase64Alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz0123456789+/";

static inline void a3_to_a4(unsigned char * a4, unsigned char * a3) {
	a4[0] = (a3[0] & 0xfc) >> 2;
	a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
	a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
	a4[3] = (a3[2] & 0x3f);
}

static inline void a4_to_a3(unsigned char * a3, unsigned char * a4) {
	a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
	a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);
	a3[2] = ((a4[2] & 0x3) << 6) + a4[3];
}

static inline unsigned char b64_lookup(unsigned char c) {
	if(c >='A' && c <='Z') return c - 'A';
	if(c >='a' && c <='z') return c - 71;
	if(c >='0' && c <='9') return c + 4;
	if(c == '+') return 62;
	if(c == '/') return 63;
	return 255;
}

inline void encodeBase64(std::istream& in, std::ostream& out) {
	size_t i = 0;
	size_t j = 0;
	unsigned char a3[3];
	unsigned char a4[4];
	char outBuf[4];

	const size_t localBufSize(16);
	char localBuf[localBufSize];

	while(!in.eof()) {
		in.get(&localBuf[0], localBufSize-1);
		auto len = in.gcount();

		for(size_t it = 0; it < len; ++it) {
			a3[i++] = localBuf[it];
			if(i == 3) {
				a3_to_a4(a4, a3);

				for(i = 0; i < 4; i++) {
					outBuf[i] = kBase64Alphabet[a4[i]];
				}
				out.write(outBuf, 4u);

				i = 0;
			}
		}
	}

	if(i) {
		for(j = i; j < 3; j++) {
			a3[j] = '\0';
		}

		a3_to_a4(a4, a3);

		for(j = 0; j < i + 1; j++) {
			out<<kBase64Alphabet[a4[j]];
		}

		while((i++ < 3)) {
			out<<'=';
		}
	}
}

inline void decodeBase64(std::istream& in, std::ostream& out) {
	size_t i = 0;
	size_t j = 0;
	unsigned char a3[3];
	unsigned char a4[4];
	char outBuf[3];

	const size_t localBufSize(16);
	char localBuf[localBufSize];

	while(!in.eof()) {
		in.get(&localBuf[0], localBufSize-1);
		auto len = in.gcount();

		for(size_t it = 0; it < len; ++it) {
			if(localBuf[it] == '=') {
				goto endWhile;
			}

			a4[i++] = localBuf[it];
			if(i == 4) {
				for(i = 0; i <4; i++) {
					a4[i] = b64_lookup(a4[i]);
				}

				a4_to_a3(a3,a4);

				for(size_t jt = 0; jt < 3; ++jt) {
					outBuf[jt] = a3[jt];
				}

				out.write(outBuf, 3u);

				i = 0;
			}
		}
	}
	endWhile:

	/*while(input_len--) {
		if(*input == '=') {
			break;
		}


		a4[i++] = *(input++);
		if(i == 4) {
			for(i = 0; i <4; i++) {
				a4[i] = b64_lookup(a4[i]);
			}

			a4_to_a3(a3,a4);

			for(i = 0; i < 3; i++) {
				(*out)[dec_len++] = a3[i];
			}

			i = 0;
		}
	}*/

	if(i) {
		for(j = i; j < 4; j++) {
			a4[j] = '\0';
		}

		for(j = 0; j < 4; j++) {
			a4[j] = b64_lookup(a4[j]);
		}

		a4_to_a3(a3,a4);

		for(j = 0; j < i - 1; j++) {
			//(*out)[dec_len++] = a3[j];
			out<<a3[j];
		}
	}
}
