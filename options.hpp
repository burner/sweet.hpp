// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
#ifndef SWEET_OPTIONS_HPP
#define SWEET_OPTIONS_HPP
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include <utility>
#include <iostream>
#include <ostream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include "format.hpp"
#include "conv.hpp"

class Options {
	struct Option {
		std::string s, l, d;
		Option(const std::string& shr, const std::string& lo, 
				const std::string& de) : s(shr), l(lo), d(de) {
		}

		void toCmdLine(size_t ls, size_t ll) {
			size_t cur = 0;
			std::cout<<std::right;
			std::cout<<std::setw(ls)<<((s.size() == 0) ? std::string(" ") + 
				"  " : (s + ", "));
			std::cout<<std::setw(ll)<<((l.size() == 0) ? std::string(" ") +
				"  " : (l + ": "));
			//cur = s.size() + 4 + l.size();
			cur = ls + ll;
			for(auto it : d) {
				if(cur % 77 == 0) {
					if(it != ' ') {
						std::cout<<'-';
					}
					std::cout<<"\n";
					cur = 0;
					for(size_t i = 0; i < ls+ll; ++i) {
						std::cout<<' ';
						++cur;
					}
				}
				++cur;
				std::cout<<it;
			}

			std::cout<<"\n\n";
		}	
	};

	typedef std::unordered_multimap<std::string,size_t>	MapType;
	typedef MapType::iterator 							IterType;
	typedef std::vector<Option> 						Opts;

public:
	Options(int c, char** v, const std::string& desc = "") :
			description(desc) {
		for(int i = 0; i < c; ++i) {
			argv.push_back(v[i]);
			mapping.insert(std::make_pair(std::string(v[i]), argv.size()-1));
		}
	}

	std::pair<
		std::pair<IterType, IterType>, 
		std::pair<IterType, IterType>
	> getIterator(const std::string& s, const std::string& l, 
			const std::string& d, int cnt) {
		opts.push_back(Option(s, l, d));
		auto sit = mapping.equal_range(s);
		auto lit = mapping.equal_range(l);
		if(std::distance(sit.first, sit.second) >= cnt && 
				std::distance(lit.first, lit.second) >= cnt) {
			throw std::logic_error(std::string(
				"Single option found by both short and long optionname: ") + s +
				std::string(" and ") + l);
		} else if(std::distance(sit.first, sit.second) > cnt) {
			throw std::logic_error(std::string(
				"Single option found by multiple time by short optionname: ") 
				+ s);
		} else if(std::distance(lit.first, lit.second) > cnt) {
			throw std::logic_error(std::string(
				"Single option found by multiple time by long optionname: ") 
				+ l);
		}

		return std::make_pair(sit,lit);
	}

	template<typename T>
	Options& get(const std::string& s, const std::string& l, const
			std::string& d, T& t) {
		auto it = getIterator(s, l, d, 1);
		auto sit = it.first;
		auto lit = it.second;
		if(std::distance(sit.first, sit.second) == 1) {
			if(std::is_same<T,bool>::value) {
				t = true;
			} else if(sit.first->second < argv.size()) {
				t = to<T>(argv[sit.first->second+1]);
			}
		} else if(std::distance(lit.first, lit.second) == 1) {
			if(std::is_same<T,bool>::value) {
				t = true;
			} else if(lit.first->second < argv.size()) {
				t = to<T>(argv[lit.first->second+1]);
			} 
		}
		return *this;
	}

	template<typename T>
	Options& getMultiple(const std::string& s, const std::string& l, const
			std::string& d, std::vector<T>& t) {
		opts.push_back(Option(s, l, d));		
		auto sit = mapping.equal_range(s);
		auto lit = mapping.equal_range(l);
		for(; sit.first != sit.second; ++sit.first) {
			t.push_back(to<T>(argv[sit.first->second+1]));
		}
		for(; lit.first != lit.second; ++lit.first) {
			t.push_back(to<T>(argv[lit.first->second+1]));
		}
		return *this;
	}

	inline void finalize() {
		auto sit = mapping.equal_range("-h");
		auto lit = mapping.equal_range("--help");

		// --help or -h not set
		if(sit.first == sit.second && lit.first == lit.second) {
			return;
		}

		if(description != "") {
			std::cout<<description<<"\n\n";
		}

		size_t mas = 0;
		size_t mal = 0;
		std::for_each(opts.begin(), opts.end(), [&mas,&mal](Option& a) {
				mas = mas < a.s.size() ? a.s.size() : mas;
				mal = mal < a.l.size() ? a.l.size() : mal;
		}); 

		mas+=2;
		mal+=2;

		std::for_each(opts.begin(), opts.end(), 
			[mas,mal](Option& o) { o.toCmdLine(mas,mal); });
		
	}

private:
	const std::string			description;
	Opts				 		opts;
	std::set<int> 				used;
	std::vector<std::string>	argv;
	MapType						mapping;
};
#endif
