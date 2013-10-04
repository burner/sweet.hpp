// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
#ifndef SWEET_BENCHMARK
#define SWEET_BENCHMARK

#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <math.h>

struct Bench {
#ifndef SWEET_NO_BENCHMARK
	std::chrono::time_point<std::chrono::system_clock> strt;
	std::chrono::time_point<std::chrono::system_clock> stp;
#endif

	inline Bench() 
#ifndef SWEET_NO_BENCHMARK
		: strt(std::chrono::system_clock::now()) 
#endif
	{}

	inline void stop() {
#ifndef SWEET_NO_BENCHMARK
		stp = std::chrono::system_clock::now();
#endif
	}

	inline size_t milli() const {
#ifndef SWEET_NO_BENCHMARK
		return std::chrono::duration_cast<
			std::chrono::milliseconds
		>(stp-strt).count();
#else
		return 0;
#endif
	}

	inline size_t micro() const {
#ifndef SWEET_NO_BENCHMARK
		return std::chrono::duration_cast<
			std::chrono::microseconds
		>(stp-strt).count();
#else
		return 0;
#endif
	}

	inline size_t second() const {
#ifndef SWEET_NO_BENCHMARK
		return std::chrono::duration_cast<
			std::chrono::seconds
		>(stp-strt).count();
#else
		return 0;
#endif
	}
};

class Benchmark {
private:
	static std::vector<Benchmark*>& getBenchClasses() {
		static std::vector<Benchmark*> baseCls;
		return baseCls;
	}
public:
	std::string name;
	std::string prettyFunc;
	std::string filename;
	unsigned long line;
	unsigned long cnt;
	unsigned long time;

	inline Benchmark() {}
	inline Benchmark(const std::string& n, const std::string& pf, const std::string& fn, int l) : 
			name(n), prettyFunc(pf), filename(fn), line(l), cnt(0), time(0) {
		Benchmark::getBenchClasses().push_back(this);
	}

	inline void saveTimeAndIncCounter(unsigned long time) { 
		__sync_add_and_fetch(&this->cnt, static_cast<unsigned long>(1));
		__sync_add_and_fetch(&this->time, time);
	}

	static inline std::string sname(const std::string& str) {
		size_t idx(str.rfind('/'));
		if(idx != std::string::npos) {
			std::string ret(str);
			ret.erase(0, idx+1);	
			return ret;
		} else
			return std::string(str);
	}

	static inline std::vector<Benchmark> getTimeConsumer() {
		std::vector<Benchmark> ret;
		for(auto it : Benchmark::getBenchClasses()) {
			ret.push_back(*it);
		}
		std::sort(ret.begin(), ret.end(), [](const Benchmark& a, const Benchmark& b) {
			return a.time > b.time;
		});
		return ret;
	}

	static inline void printResults() {
		auto rslt = Benchmark::getTimeConsumer();
		const std::string funcName = "Function Name";
		const std::string time = "Time in Millisec";
		const std::string line = "Linenumber";
		const std::string file = "Filename";
		size_t nLen = funcName.size(), tLen = time.size(); 
		size_t fLen = file.size(), lLen = line.size();
		for(auto& it : rslt) {
			nLen = std::max(nLen, it.name.size());
			tLen = std::max(tLen, it.time);
			fLen = std::max(fLen, it.filename.size());
			lLen = std::max(lLen, static_cast<size_t>(log10(it.line)));
		}
		tLen = std::max(static_cast<size_t>(log10(tLen)), time.size());
		++nLen; ++tLen; ++fLen; ++lLen;

		std::cout<<std::setw(nLen)<<funcName<<" "<<std::setw(tLen)<<time<<" "
			<<std::setw(fLen)<<file<<" "<<std::setw(lLen)<<line<<std::endl;
		for(auto& it : rslt) {
			std::cout<<std::setw(nLen)<<it.name<<" "<<std::setw(tLen)<<it.time<<" "
			<<std::setw(fLen)<<sname(it.filename)<<" "<<std::setw(lLen)<<it.line
			<<std::endl;
		}
	}
};

class C {
	Benchmark* store;
	std::chrono::time_point<std::chrono::system_clock> strt;
public:
	inline C(Benchmark* s) : store(s), strt(std::chrono::system_clock::now()) {}
	inline ~C() { 
		store->saveTimeAndIncCounter(
			std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now()-strt
			).count()
		);
	}
};

#define CONCAT_IMPL( x, y ) x##y

#ifndef SWEET_NO_BENCHMARK
#define BENCH(name)
#else
#define BENCH(name) static Benchmark name (#name,__PRETTY_FUNCTION__,__FILE__,__LINE__); \
C CONCAT_IMPL(name, __COUNTER__)(& name)
#endif

#endif
