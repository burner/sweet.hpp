#include <vector>

#include <stringfunctions.hpp>
#include <unit.hpp>
#include <benchmark.hpp>

typedef std::pair<
		std::vector<std::string>,
		std::vector<std::string>
	> ToCmpVec;

static ToCmpVec genToCmp(const size_t iLen, const size_t jLen, const size_t offset) {
	auto chars = std::string("abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ");

	ToCmpVec ret;

	for(size_t i = 0; i < iLen; ++i) {
		for(size_t j = 1; j < jLen; j *= 2) {
			std::string tmp;
			for(size_t k = 0; k < j; ++k) {
				tmp.push_back(chars[(i+j+offset)%chars.size()]);	
			}

			ret.first.push_back(tmp);
			ret.second.push_back(tmp);
		}
	}	

	return ret;
}

UNITTEST(ooo) {
	auto a = std::string("oooooooooooooooooooooooooooooooo");
	auto b = std::string("oooooooooooooooooooooooooooooooo");

	bool t = sweet::stringCmpSSE(a,b);
	AS_T(t);
}

UNITTEST(test) {
	BENCH(test);
	auto tc = genToCmp(200, 128, 32);
	size_t cntTrue = 0;
	for(auto& it : tc.first) {
		for(auto& jt : tc.second) {
			cntTrue += (it == jt);
			AS_EQ_C(sweet::stringCmpNaive(it, jt), it == jt, [&]() {
				std::cout<<it<<std::endl;
				std::cout<<jt<<std::endl;
			});
			AS_EQ_C(it == jt, sweet::stringCmpSSE(it,jt), [&]() {
				std::cout<<it<<std::endl;
				std::cout<<jt<<std::endl;
			});
			AS_EQ(sweet::stringCmpNaive(it, jt), sweet::stringCmpSSE(it,jt));
		}
	}

	AS_T(cntTrue > 0);
}

std::vector<bool> benchNaive(const ToCmpVec& tc) {
	BENCH(benchNaive);
	std::vector<bool> ret;
	ret.reserve(tc.first.size() * tc.second.size());

	size_t idx = 0;
	for(auto& it : tc.first) {
		for(auto& jt : tc.second) {
			ret[idx] = sweet::stringCmpNaive(it, jt);
		}
	}

	return ret;
}

std::vector<bool> benchSSE(const ToCmpVec& tc) {
	BENCH(benchSSE);
	std::vector<bool> ret;
	ret.reserve(tc.first.size() * tc.second.size());

	size_t idx = 0;
	for(auto& it : tc.first) {
		for(auto& jt : tc.second) {
			ret[idx] = sweet::stringCmpSSE(it, jt);
		}
	}

	return ret;
}

std::vector<bool> benchInbuild(const ToCmpVec& tc) {
	BENCH(benchInbuild);
	std::vector<bool> ret;
	ret.reserve(tc.first.size() * tc.second.size());

	size_t idx = 0;
	for(auto& it : tc.first) {
		for(auto& jt : tc.second) {
			ret[idx] = (it == jt);
		}
	}

	return ret;
}

UNITTEST(benchmark) {
	for(int i = 0; i < 43; ++i) {
		auto tc = genToCmp(257+i, 137+i, i);
		auto inBuild = benchInbuild(tc);
		auto naiveBuild = benchNaive(tc);
		auto sseBuild = benchSSE(tc);
		naiveBuild = benchNaive(tc);
		sseBuild = benchSSE(tc);
		inBuild = benchInbuild(tc);

		const size_t len = inBuild.size();
		for(size_t i = 0; i < len; ++i) {
			AS_EQ(inBuild[i], naiveBuild[i]);	
			AS_EQ(inBuild[i], sseBuild[i]);	
		}
	}
}
