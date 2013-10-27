#include <vector>
#include <random>
#include <ostream>
#include <iomanip>
#include <iostream>
#include <format.hpp>
#include <benchmark.hpp>
#include <unit.hpp>
#include <vectorclass.h>

std::ostream& operator<<(std::ostream& out, const std::vector<std::vector<float>>& vec) {
	for(auto& it : vec) {
		for(auto &jt : it) {
			out<<std::setw(7)<<std::setprecision(4)<<jt<<" ";
		}
		out<<std::endl;
	}
	return out;
}

void fill(std::vector<std::vector<float>>& vec) { 
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 10);
	for(auto& jt : vec) {
		for(auto& it : jt) {
			it = dis(gen);
		}
	}
}

std::vector<std::vector<float>> transpose(const std::vector<std::vector<float>>& o) {
	const size_t N = o.size();
	std::vector<std::vector<float>> trans = o;
	for(size_t c = 0 ; c < N ; ++c) {
		for(size_t d = 0 ; d < N ; ++d ) {
		   trans[d][c] = o[c][d];
		}
	}
	return trans;
}

// without transpose and sse
void simpleMul3(const std::vector<std::vector<float>>& vec, const std::vector<std::vector<float>>& o, 
		std::vector<std::vector<float>>& rslt) {
	BENCH(simpleMul3);

	const size_t N = vec.size();

	size_t i, j, k;
	//size_t Nk = N / 4;
	float sum;
	for(i = 0; i < N; ++i) {
		for(j = 0; j < N; ++j) {
			sum = 0.0f;
			for(k = 0; k < N; k+=8) {
				Vec8f a, b;
				a.load(&vec[i][k]);
				b.load(&o[j][k]);

				a*= b;
				sum += horizontal_add(a);
			}
			for(; k < N; ++k) {
				sum += vec[i][k] * o[j][k];
			}
			rslt[i][j] = sum;
		}
	}
}

// without transpose
void simpleMul4(const std::vector<std::vector<float>>& vec, const std::vector<std::vector<float>>& o, 
		std::vector<std::vector<float>>& rslt) {
	BENCH(simpleMul4);

	const size_t N = vec.size();

	size_t i, j, k;
	float sum;
	for(i = 0; i < N; ++i) {
		for(j = 0; j < N; ++j) {
			sum = 0.0f;
			for(k = 0; k < N; k+=4) {
				sum += vec[i][k] * o[j][k];
				sum += vec[i][k+1] * o[j][k+1];
				sum += vec[i][k+2] * o[j][k+2];
				sum += vec[i][k+3] * o[j][k+3];
			}
			for(; k < N; ++k) {
				sum += vec[i][k] * o[j][k];
			}
			rslt[i][j] = sum;
		}
	}
}

// without transpose
void simpleMul2(const std::vector<std::vector<float>>& vec, const std::vector<std::vector<float>>& wo,
		std::vector<std::vector<float>>& rslt) {
	BENCH(simpleMul2);

	const size_t N = vec.size();

	auto o = transpose(wo);
	size_t i, j, k;
	float sum;
	for(i = 0; i < N; ++i) {
		for(j = 0; j < N; ++j) {
			sum = 0.0f;
			for(k = 0; k < N; ++k) {
				sum += vec[i][k] * o[j][k];
			}
			rslt[i][j] = sum;
		}
	}
}

void simpleMul(const std::vector<std::vector<float>>& vec, const std::vector<std::vector<float>>& o, 
		std::vector<std::vector<float>>& rslt) {
	BENCH(simpleMul);

	const size_t N = vec.size();

	size_t i, j, k;
	float sum;
	for(i = 0; i < N; ++i) {
		for(j = 0; j < N; ++j) {
			sum = 0.0f;
			for(k = 0; k < N; ++k) {
				sum += vec[i][k] * o[k][j];
			}
			rslt[i][j] = sum;
		}
	}
}

void simpleFill(std::vector<std::vector<float>>& vec, size_t N) {
	vec.resize(N, std::vector<float>(N, std::numeric_limits<float>::quiet_NaN()));
}

void compare(const std::vector<std::vector<float>>& a, const std::vector<std::vector<float>>& b) {
	ASSERT_EQ(a.size(), b.size());
	for(size_t i = 0; i < a.size(); ++i) {
		ASSERT_EQ(a[i].size(), b[i].size());
		for(size_t j = 0; j < a[i].size(); ++j) {
			ASSERT_EQ(a[i][j], b[i][j]);
		}
	}
}

int main() {
	const size_t N=2048;

	for(size_t z = 0; z < 10; ++z) {
		std::vector<std::vector<float>> R(N, std::vector<float>(N, std::numeric_limits<float>::quiet_NaN()));
		fill(R);
		//std::cout<<R<<std::endl;
		std::vector<std::vector<float>> A;
		simpleFill(A, N);

		auto T = transpose(R);
		simpleMul2(R,T, A);
		auto oldA = A;
		//std::cout<<A<<std::endl;
		simpleMul3(R,R, A);
		compare(oldA, A);
		//std::cout<<A<<std::endl;
	}

	Benchmark::printResults();
}
