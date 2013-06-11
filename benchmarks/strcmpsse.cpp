#include <string>
#include <vector>
#include <fstream>
#include <logger.hpp>
#include <algorithm>
#include <benchmark.hpp>

/*
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
	if(p1[ResultIndex]<p2[ResultIndex])  return ‐1;
	if(p1[ResultIndex]>p2[ResultIndex])  return 1;
	return 0;
}*/

int main() {
	std::ifstream infile("text3");
	std::vector<std::string> words(10000);
	std::string line;
	while (std::getline(infile, line)) {
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
			if(words[i] == shuffle[j]) {
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
