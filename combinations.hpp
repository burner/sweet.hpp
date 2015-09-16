#pragma once

#include <vector>
#include <cstddef>

// http://stackoverflow.com/questions/9430568/generating-combinations-in-c

namespace sweet {

template<typename RAC = typename std::vector<int>>
struct Combinations {
	//typedef std::vector<int> Combination;
	typedef RAC Combination;

	// initialize status
	inline Combinations(int nN, int nR) : completed(nN < 1 || nR > nN), generated(0),
			N(nN), R(nR)
	{
		for(int c = 0; c < nR; ++c) {
	 		curr.push_back(c);
		}
	}
	
	// true while there are more solutions
	bool completed;
	
	// count how many generated
	int generated;
	
	// get current and compute next combination
	inline Combination next() {
		Combination ret = curr;
		
		// find what to increment
		completed = true;
		for(int i = R - 1; i >= 0; --i) {
			if(curr[static_cast<size_t>(i)] < N - R + i) {
				int j = curr[static_cast<size_t>(i)] + 1;
				while (i <= R) {
					curr[static_cast<size_t>(i++)] = j++;
				}
				completed = false;
				++generated;
				break;
			}
		}
		
		return ret;
	}

private:

	int N;
   	int R;
	Combination curr;
};
}
