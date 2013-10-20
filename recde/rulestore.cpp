#include <rulestore.hpp>

std::ostream& operator<<(std::ostream& ss, const FirstSets& f) {
	for(auto& it : f) {
		format(ss, "%s :\n\t", it.first);
		size_t cnt = 0;
		for(auto& jt : it.second) {
			ss<<jt<<" ";	
			cnt += jt.size();
			if(cnt > 70) {
				ss<<"\n\t";
				cnt = 0;
			}
		}
		ss<<"\n\n";
	}
	return ss;
}
