// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
#ifndef SWEET_BENCHMARK
#define SWEET_BENCHMARK

#include <chrono>
#include <vector>
//#include <atomic>
//#include <atomic.h>
#include <algorithm>

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

class Bbase {
private:
	static std::vector<Bbase*> baseCls;

public:
	std::string name;
	std::string prettyFunc;
	std::string filename;
	int line;
	unsigned long cnt;
	unsigned long time;

	inline Bbase() {}
	inline Bbase(const std::string& n, const std::string& pf, const std::string& fn, int l) : 
			name(n), prettyFunc(pf), filename(fn), line(l), cnt(0), time(0) {
		Bbase::baseCls.push_back(this);
	}

	inline void saveTimeAndIncCounter(unsigned long time) { 
		__sync_add_and_fetch(&this->cnt, static_cast<unsigned long>(1));
		__sync_add_and_fetch(&this->time, time);
	}

	static inline std::vector<Bbase*> getBaseCls() {
		return Bbase::baseCls;
	}

	static inline std::vector<Bbase> getTimeConsumer() {
		size_t oldSize = Bbase::baseCls.size();
		std::vector<Bbase> ret;
		for(auto it : Bbase::baseCls) {
			ret.push_back(*it);
		}
		std::sort(ret.begin(), ret.end(), [](const Bbase& a, const Bbase& b) {
			return a.time > b.time;
		});
		std::cout<<oldSize<<" "<<ret.size()<<std::endl;
		return ret;
	}
};

std::vector<Bbase*> Bbase::baseCls;

class C {
	Bbase* store;
	std::chrono::time_point<std::chrono::system_clock> strt;
public:
	inline C(Bbase* s) : store(s), strt(std::chrono::system_clock::now()) {}
	inline ~C() { 
		store->saveTimeAndIncCounter(
			std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now()-strt
			).count()
		);
	}
};

#define CONCAT_IMPL( x, y ) x##y

#define BENCH(name) static Bbase name (#name,__PRETTY_FUNCTION__,__FILE__,__LINE__); \
C CONCAT_IMPL(name, __COUNTER__)(& name)

#endif
