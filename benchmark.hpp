// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
#ifndef SWEET_BENCHMARK
#define SWEET_BENCHMARK

#include <chrono>

struct Bench {
	std::chrono::time_point<std::chrono::system_clock> strt;
	std::chrono::time_point<std::chrono::system_clock> stp;

	inline Bench() : strt(std::chrono::system_clock::now()) {}

	inline void stop() {
		stp = std::chrono::system_clock::now();
	}

	inline size_t milli() const {
		return std::chrono::duration_cast<
			std::chrono::milliseconds
		>(stp-strt).count();
	}

	inline size_t micro() const {
		return std::chrono::duration_cast<
			std::chrono::microseconds
		>(stp-strt).count();
	}

	inline size_t second() const {
		return std::chrono::duration_cast<
			std::chrono::seconds
		>(stp-strt).count();
	}
};
#endif
