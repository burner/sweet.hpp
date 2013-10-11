// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
#ifndef SWEET_BENCHMARK_HPP
#define SWEET_BENCHMARK_HPP

#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <math.h>

#ifndef _WIN32
inline void __cpuid(int CPUInfo[4],int InfoType) {
    __asm__ __volatile__ (
        "cpuid":
        "=a" (CPUInfo[0]),
        "=b" (CPUInfo[1]),
        "=c" (CPUInfo[2]),
        "=d" (CPUInfo[3]) :
        "a" (InfoType)
    );
}
#endif

#ifdef __i386
__inline__ uint64_t __rdtsc() {
  uint64_t x;
  __asm__ volatile ("rdtsc" : "=A" (x));
  return x;
}
#elif __amd64
__inline__ uint64_t __rdtsc() {
  uint64_t a, d;
  __asm__ volatile ("rdtsc" : "=a" (a), "=d" (d));
  return (d<<32) | a;
}
#endif

inline long long readTicks() {
	int dummy[4];
	volatile int DontSkip;
	long long clock;
	__cpuid(dummy, 0);
	DontSkip = dummy[0];
	clock = __rdtsc();
	__cpuid(dummy, 0);
	DontSkip = dummy[0];
	DontSkip = DontSkip+0;
	return clock;
}

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
	long long ticks;

	inline Benchmark() {}
	inline Benchmark(const std::string& n, const std::string& pf, const std::string& fn, int l) : 
			name(n), prettyFunc(pf), filename(fn), line(l), cnt(0), time(0), ticks(0) {
		Benchmark::getBenchClasses().push_back(this);
	}

	inline void saveTimeAndIncCounter(unsigned long time, long long tick) { 
		__sync_add_and_fetch(&this->cnt, static_cast<unsigned long>(1));
		__sync_add_and_fetch(&this->time, time);
		__sync_add_and_fetch(&this->ticks, tick);
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
		const std::string funcName = "Function Name:";
		const std::string time = "Time in Millisec:";
		const std::string line = "Linenumber:";
		const std::string file = "Filename:";
		const std::string numTicks = "Number of Ticks:";
		size_t nLen = funcName.size();
		unsigned long tLen = static_cast<long long>(time.size());
		size_t fLen = file.size();
		unsigned long lLen = static_cast<unsigned long>(line.size());
		long long tiLen = 0;

		for(auto& it : rslt) {
			it.filename = sname(it.filename);
			nLen = nLen > it.name.size() ? nLen : it.name.size();
			fLen = fLen > it.filename.size() ? fLen : it.filename.size();
			tLen = tLen > it.time ? tLen : it.time;
			lLen = static_cast<unsigned long>(lLen > static_cast<unsigned long>(log10(it.line)) ? 
				lLen : log10(it.line)
			);
			tiLen = static_cast<long long>(tiLen > static_cast<long long>(log10(it.ticks)) ? 
				tiLen : log10(it.ticks)
			);
		}
		tLen =  std::max(static_cast<long long>(log10(tLen)), static_cast<long long>(time.size()));
		tiLen = std::max(static_cast<long long>(log10(tiLen)),static_cast<long long>(numTicks.size()));
		++nLen; ++tLen; ++fLen; ++lLen;

		std::cout<<std::setw(nLen)<<funcName<<" "<<std::setw(tLen)<<time<<" "
			<<std::setw(tiLen)<<numTicks<<" "
			<<std::setw(fLen)<<file<<" "<<std::setw(lLen)<<line<<std::endl;
		for(auto& it : rslt) {
			std::cout<<std::setw(nLen)<<it.name<<" "
			<<std::setw(tLen)<<it.time<<" "
			<<std::setw(tiLen)<<it.ticks<<" "
			<<std::setw(fLen)<<it.filename<<" "
			<<std::setw(lLen)<<it.line<<std::endl;
		}
	}
};

class C {
	Benchmark* store;
	std::chrono::time_point<std::chrono::system_clock> strt;
	long long ticks;
public:
	inline C(Benchmark* s) : store(s), strt(std::chrono::system_clock::now()),
   		ticks(readTicks()) 
	{
	}
	inline ~C() { 
		store->saveTimeAndIncCounter(
			std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now()-strt
			).count(),
			readTicks() - ticks			
		);
	}
};

#define CONCAT_IMPL( x, y ) x##y

#ifdef SWEET_NO_BENCHMARK
#define BENCH(name)
#else
#define BENCH(name) static Benchmark CONCAT_IMPL(name, fooBar_youCantGuessMe) \
(#name,__PRETTY_FUNCTION__,__FILE__,__LINE__); \
C CONCAT_IMPL(name, __COUNTER__)(& CONCAT_IMPL(name, fooBar_youCantGuessMe))
#endif

#endif
