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

template<size_t N, size_t I=0>
struct hash_calc {
   //static constexpr size_t apply (const char (&s)[N]) {
   static constexpr size_t apply (const char* s) {
      return  (hash_calc<N, I+1>::apply(s) ^ s[I]) * 16777619u;
   };
};

template<size_t N>
struct hash_calc<N,N> {
   //static constexpr size_t apply (const char (&)[N]) {
   static constexpr size_t apply (const char* z) {
      return  2166136261u;
   };
};

template<size_t N>
constexpr size_t hash ( const char (&s)[N] ) {
   return hash_calc<N>::apply(s);
}

class Bbase {
public:
	virtual void begin() { std::cout<<"Base Begin"<<std::endl; }
	virtual void end() { std::cout<<"Base End"<<std::endl; }
};

template<size_t v>
class B : public Bbase {
	std::string name;
public:
	B(const std::string n) : name(n) {}
	inline const std::string& getName() const { return name; }
	void begin() { std::cout<<"Begin"<<std::endl; }
	void end() { std::cout<<"End"<<std::endl; }
};

class C {
	Bbase* store;
public:
	inline C(Bbase* s) : store(s) { store->begin(); }
	inline ~C() { store->end(); }
};

#define BENCH(name) B<hash(#name)> name (__PRETTY_FUNCTION__); C __timeTakerBench(& name)
#endif
