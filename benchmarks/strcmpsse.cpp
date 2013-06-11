#include <string>
#include <vector>
#include <fstream>
#include <logger.hpp>
#include <algorithm>
#include <benchmark.hpp>
#include "smmintrin.h"

/*
strcmpSSE42:
        push    ebp
        mov     ebp, esp
 
        mov     eax, [ebp+8]    ; Arg1: copy s1(pointer to string) to eax
        mov     edx, [ebp+12]   ; Arg2: copy s2(pointer to string) to edx
        ;
        ; Subtract s2(edx) from s1(eax). This admititedly looks odd, but we
        ; can now use edx to index into s1 and s2. As we adjust edx to move
        ; forward into s2, we can then add edx to eax and this will give us
        ; the comparable offset into s1 i.e. if we take edx + 16 then:
        ;
        ;       edx     = edx + 16              = edx + 16
        ;       eax+edx = eax -edx + edx + 16   = eax + 16
        ;
        ; therefore edx points to s2 + 16 and eax + edx points to s1 + 16.
        ; We thus only need one index, convoluted but effective.
        ;
        sub     eax, edx
        sub     edx, 16         ; Avoid extra jump in main loop
 
strcmpLoop:
        add     edx, 16
        movdqu  xmm0, [edx]
        ;
        ; IMM8[1:0]     = 00b
        ;       Src data is unsigned bytes(16 packed unsigned bytes)
        ; IMM8[3:2]     = 10b
        ;       We are using Equal Each aggregation
        ; IMM8[5:4]     = 01b
        ;       Negative Polarity, IntRes2      = -1 XOR IntRes1
        ; IMM8[6]       = 0b
        ;       ECX contains the least significant set bit in IntRes2
        ;
        pcmpistri       xmm0, [edx+eax], 0011000b
        ;
        ; Loop while ZF=0 and CF=0:
        ;
        ;       1) We find a null in s1(edx+eax) ZF=1
        ;       2) We find a char that does not match CF=1
        ;
        ja      strcmpLoop
 
        ;
        ; Jump if CF=1, we found a mismatched char
        ;
        jc      strcmpDiff
 
        ;
        ; We terminated loop due to a null character i.e. CF=0 and ZF=1
        ;
        xor     eax, eax        ; They are equal so return zero
        jmp     exitStrcmp
 
strcmpDiff:
        add     eax, edx        ; Set offset into s1 to match s2
        ;
        ; ecx is offset from current poition where two strings do not match,
        ; so copy the respective non-matching byte into eax and edx and fill
        ; in remaining bits w/ zero.
        ;
        movzx   eax, byte[eax+ecx]
        movzx   edx, byte[edx+ecx]
        ;
        ; If s1 is less than s2 return integer less than zero, otherwise return
        ; integer greater than zero.
        ;
        sub     eax, edx
 
exitStrcmp:
        pop     ebp
        ret
*/

int sse_strcmp(const char *p1, const char *p2)  {
	const int mode = _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_BIT_MASK | _SIDD_NEGATIVE_POLARITY;
	__m128i smm1 = _mm_loadu_si128 ((__m128i *) p1);
	__m128i smm2 = _mm_loadu_si128 ((__m128i *) p2);
	int ResultIndex;
	while (1) {
		ResultIndex = _mm_cmpistri (smm1, smm2, mode );
		if (ResultIndex != 16) { break; }
		p1 = p1+16;
		p2 = p2+16;
		smm1 = _mm_loadu_si128 ((__m128i *)p1);
		smm2 = _mm_loadu_si128 ((__m128i *)p2);
	}
	p1 = (char *) & smm1;
	p2 = (char *) & smm2;
	if(p1[ResultIndex] < p2[ResultIndex]) return -1;
	if(p1[ResultIndex] > p2[ResultIndex]) return 1;
	return 0;
}

int main() {
	std::ifstream infile("text3");
	std::vector<std::string> words(10000);
	std::string line;
	while (std::getline(infile, line)) {
		if(line.empty() && line == "") {
			continue;
		}
		words.push_back(line);
	}

	std::vector<std::string> shuffle(words.begin(), words.end());
	std::random_shuffle(shuffle.begin(), shuffle.end());	

	std::vector<size_t> wf(words.size());
	std::vector<size_t> ws(words.size());

	Bench normal;
	const size_t wordsSize = words.size();
	for(size_t i = 0; i < wordsSize; ++i) {
		for(size_t j = 0; j < wordsSize; ++j) {
			if(words[i] == shuffle[j]) {
				wf[i] = j;
				break;
			}
		}
	}
	normal.stop();
	LOG("normal %f", normal.milli());
	LOG("%u %u", words.size(), shuffle.size());

	Bench sse;
	for(size_t i = 0; i < wordsSize; ++i) {
		for(size_t j = 0; j < wordsSize; ++j) {
			if(sse_strcmp(words[i].c_str(), shuffle[j].c_str())) {
				ws[i] = j;
				break;
			}
		}
	}
	sse.stop();
	LOG("normal %f", sse.milli());

	for(size_t i = 0; i < wordsSize; ++i) {
		if(wf[i] != ws[i]) {
			return 1;
		}
	}
	return 0;
}
