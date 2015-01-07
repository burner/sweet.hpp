// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
#pragma once

#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <memory>
#include <mutex>
#include <algorithm>
#include <math.h>
#include <int128.hpp>

namespace sweet {

inline uint64_t rdtsc_time(void) {
   unsigned long long int x;
   unsigned a, d;

   __asm__ volatile("rdtsc" : "=a" (a), "=d" (d));

   return static_cast<uint64_t>(a) | (static_cast<uint64_t>(d) << 32);
}

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

/*
#ifdef __clang__
#ifdef __i386
__inline__ __attribute__((gnu_inline)) uint64_t __rdtsc() {
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
#endif*/

inline int64_t readTicks() {
	int dummy[4];
	volatile int DontSkip;
	int64_t clock;
	__cpuid(dummy, 0);
	DontSkip = dummy[0];
	clock = rdtsc_time();
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
	long long line;
	long long cnt;
	long long time;
	//long long ticks;
	std::shared_ptr<std::mutex> tickMutex;
	int128 ticks;

	inline Benchmark() : tickMutex(std::make_shared<std::mutex>()) {}
	inline Benchmark(const std::string& n, const std::string& pf, 
			const std::string& fn, int l) : name(n), prettyFunc(pf), 
			filename(fn), line(l), cnt(0), time(0), 
			tickMutex(std::make_shared<std::mutex>()), ticks(0) {
		Benchmark::getBenchClasses().push_back(this);
	}

	inline void saveTimeAndIncCounter(unsigned long timen, int128 tick) { 
		__sync_add_and_fetch(&this->cnt, static_cast<unsigned long>(1));
		__sync_add_and_fetch(&this->time, timen);
		std::lock_guard<std::mutex> lock(*tickMutex.get());
		ticks += tick;
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

	static inline long long numDigists(long long in) {
		long long n = 0;
		while(in) {
			in /= 10;
			++n;
		}
		return n;
	}

	static inline void printResults() {
		auto rslt = Benchmark::getTimeConsumer();
		const std::string funcName = "Function Name:";
		const std::string time = "Millisec spend:";
		const std::string line = "Linenumber:";
		const std::string file = "Filename:";
		const std::string numTicks = "Num of Ticks:";
		const std::string numCalls = "Num of Calls:";
		long long nLen = funcName.size();
		long long tLen = static_cast<long long>(time.size());
		long long fLen = file.size();
		long long lLen = static_cast<long long>(line.size());
		long long tiLen = 0;
		long long cLen = static_cast<long long>(numCalls.size());

		for(auto& it : rslt) {
			it.filename = sname(it.filename);
			nLen = nLen > static_cast<long long>(it.name.size()) ? nLen : 
				static_cast<long long>(it.name.size());
			fLen = fLen > static_cast<long long>(it.filename.size()) ? fLen : 
					static_cast<long long>(it.filename.size());
			tLen = tLen > it.time ? tLen : it.time;
			lLen = std::max(numDigists(it.line), lLen);
			tiLen = std::max(numDigists(it.ticks.digits()), tiLen);
			cLen = std::max(numDigists(it.cnt), cLen);
			//lLen = static_cast<long long>(lLen > static_cast<long long>(log10(it.line)+1) ? 
			//	lLen : log10(it.line)+1
			//);
			//tiLen = static_cast<long long>(tiLen > static_cast<long long>(it.ticks.digits()) ? 
			//	tiLen : it.ticks.digits()
			//);
			//cLen = static_cast<long long>(cLen > static_cast<long long>(log10(it.cnt)) ? 
			//	cLen : log10(it.cnt)
			//);
		}
		tLen =  std::max(static_cast<long long>(numDigists(tLen)), static_cast<long long>(time.size()));
		tiLen = std::max(tiLen, static_cast<long long>(numTicks.size()))+3;
		cLen = std::max(cLen ,static_cast<long long>(numCalls.size()));
		//tiLen = std::max(static_cast<long long>(tiLen),static_cast<long long>(numTicks.size()));
		//cLen = std::max(static_cast<long long>(log10(cLen)),static_cast<long long>(numCalls.size()));
		++nLen; ++tLen; ++fLen; ++lLen, ++cLen;

		std::cout<<std::setw(nLen)<<funcName<<" "<<std::setw(tLen)<<time<<" "
			<<std::setw(tiLen)<<numTicks<<" "<<std::setw(cLen)<<numCalls<<" "
			<<std::setw(fLen)<<file<<" "<<std::setw(lLen)<<line<<std::endl;
		for(auto& it : rslt) {
			std::cout<<std::setw(nLen)<<it.name<<" "
			<<std::setw(tLen)<<it.time<<" "
			<<std::setw(tiLen)<<it.ticks<<" "
			<<std::setw(cLen)<<it.cnt<<" "
			<<std::setw(fLen)<<it.filename<<" "
			<<std::setw(lLen)<<it.line<<std::endl;
		}
	}
};

class C {
	Benchmark* store;
	std::chrono::time_point<std::chrono::system_clock> strt;
	int128 ticks;
public:
	inline C(Benchmark* s) : store(s), strt(std::chrono::system_clock::now()),
   		ticks(readTicks()) 
	{
	}
	inline ~C() { 
		auto n(std::chrono::system_clock::now());
		auto dur(std::chrono::duration_cast<std::chrono::milliseconds>(
			n-strt
		));

		store->saveTimeAndIncCounter(
			//std::chrono::duration_cast<std::chrono::milliseconds>(
			//	std::chrono::system_clock::now()-strt
			//).count(),
			dur.count(),
			int128(readTicks()) - ticks			
		);
	}
};
}

#define CONCAT_IMPL( x, y ) x##y
#define CONCAT(x,y) CONCAT_IMPL(x,y)

#ifdef SWEET_NO_BENCHMARK
#define BENCH(name)
#else
#define BENCH(name) static sweet::Benchmark CONCAT_IMPL(name, fooBar_youCantGuessMe) \
(#name,__PRETTY_FUNCTION__,__FILE__,__LINE__); \
sweet::C CONCAT_IMPL(name, __COUNTER__)(& CONCAT_IMPL(name, fooBar_youCantGuessMe))
#endif

#define BENCHMARK(f) [&]() {sweet::Bench CONCAT(__secret,__LINE__); f; CONCAT(__secret,__LINE__).stop(); auto ret = CONCAT(__secret,__LINE__).milli(); return ret;}()

#define BENCHMARK_CNT(COUNTER, f) [&]() {sweet::Bench CONCAT(__secret,__LINE__); for(int CONCAT(__ITER,__LINE__) = 0; CONCAT(__ITER,__LINE__) < COUNTER; ++CONCAT(__ITER,__LINE__)) {f;} CONCAT(__secret,__LINE__).stop(); auto ret = CONCAT(__secret,__LINE__).micro(); return ret/static_cast<double>(COUNTER);}()
