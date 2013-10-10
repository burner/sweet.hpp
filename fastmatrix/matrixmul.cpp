#include <vector>
#include <random>
#include <iostream>
#include <format.hpp>
#include <benchmark.hpp>

void fill(std::vector<std::vector<float>>& vec) { 
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 10);
	for(auto& jt : vec) {
		for(auto& it : jt) {
			it = dis(gen);
		}
	}
}

void simpleMul(std::vector<std::vector<float>>& vec, std::vector<std::vector<float>>& rslt) {
	BENCH(simpleMul);

	const size_t N = vec.size();
	std::vector<std::vector<float>> trans = vec;
	for(size_t c = 0 ; c < N ; ++c) {
		for(size_t d = 0 ; d < N ; ++d ) {
		   trans[d][c] = vec[c][d];
		}
	}

	size_t i, j, k;
	float sum;
	for(i = 0; i < N; ++i) {
		for(j = 0; j < N; ++j) {
			sum = 0.0f;
			for(k = 0; k < N; ++k) {
				sum += vec[i][k] * trans[k][j];
			}
			rslt[i][j] = sum;
		}
	}
}

void simpleFill(std::vector<std::vector<float>>& vec, size_t N) {
	vec.resize(N, std::vector<float>(N, std::numeric_limits<float>::quiet_NaN()));
}

int main() {
	const size_t N=1024;

	std::vector<std::vector<float>> R(N, std::vector<float>(N, std::numeric_limits<float>::quiet_NaN()));
	fill(R);
	std::vector<std::vector<float>> A;
	simpleFill(A, N);

	simpleMul(R, A);

	Benchmark::printResults();
}
