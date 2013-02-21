#ifndef SWEET_OPTIONS_HPP
#define SWEET_OPTIONS_HPP
#include <map>
#include <vector>
#include <set>
#include <utility>
#include <iostream>
#include <string>
#include <stdexcept>

class Options {
struct Option {
	std::string s, l, d;
	Option(const std::string& shr, const std::string& lo, 
			const std::string& de) : s(shr), l(lo), d(de) {
	}
};

public:
	Options(int c, char** v) {
		for(int i = 0; i < c; ++i) {
			argv.push_back(v[i]);
			std::cout<<v[i]<<std::endl;
			mapping.insert(std::make_pair(std::string(v[i]), argv.size()-1));
		}
	}

	std::pair<std::pair<std::multimap<std::string,size_t>::iterator,
						std::multimap<std::string,size_t>::iterator>,
			  std::pair<std::multimap<std::string,size_t>::iterator,
						std::multimap<std::string,size_t>::iterator>>
	getIterator(const std::string& s, const std::string& l, 
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


	//template<typename T>
	Options& get(const std::string& s, const std::string& l, const
			std::string& d, std::string& t) {
		auto it = getIterator(s, l, d, 1);
		auto sit = it.first;
		auto lit = it.second;
		if(std::distance(sit.first, sit.second) == 1) {
			if(sit.first->second < argv.size()) {
				t = argv[sit.first->second+1];
			}
		} else if(std::distance(lit.first, lit.second) == 1) {
			if(lit.first->second < argv.size()) {
				t = argv[lit.first->second+1];
			}
		}
		return *this;
	}

	Options& getMultiple(const std::string& s, const std::string& l, const
			std::string& d, std::vector<std::string>& t) {
		/*for(auto it : mapping) {
			std::cout<<it.first<<std::endl;
		}*/
		opts.push_back(Option(s, l, d));		
		auto sit = mapping.equal_range(s);
		auto lit = mapping.equal_range(l);
		if(std::distance(sit.first, sit.second) >= 1 && 
				std::distance(lit.first, lit.second) >= 1) {
			throw std::logic_error(std::string(
				"Single option found by both short and long optionname: ") + s +
				std::string(" and ") + l);
		} else if(std::distance(sit.first, sit.second) > 1) {
			throw std::logic_error(std::string(
				"Single option found by multiple time by short optionname: ") 
				+ s);
		} else if(std::distance(lit.first, lit.second) > 1) {
			throw std::logic_error(std::string(
				"Single option found by multiple time by long optionname: ") 
				+ l);
		} else if(std::distance(sit.first, sit.second) == 1) {
			if(sit.first->second < argv.size()) {
				t.push_back(argv[sit.first->second+1]);
			}
		} else if(std::distance(lit.first, lit.second) == 1) {
			if(lit.first->second < argv.size()) {
				t.push_back(argv[lit.first->second+1]);
			}
		}
		return *this;
	}

private:
	std::vector<Option> 				opts;
	std::set<int> 						used;
	std::vector<std::string>			argv;

	std::multimap<std::string,size_t> 	mapping;
};
#endif
