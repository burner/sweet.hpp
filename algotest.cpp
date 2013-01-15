#include <vector>
#include <iostream>
#include <chrono>
#include "parallelalgo.hpp"

void fun(int& a) {
	a *= 5;
}

int main() {
	std::vector<int> v(5000000, 1);

	std::chrono::time_point<std::chrono::system_clock> start;
	start = std::chrono::system_clock::now();
	css::for_each(v.begin(), v.end(), [](int& a) {
		a *= 5;
	});
	/*std::for_each(v.begin(), v.end(), [](int& a) {
		a *= 5;
	});*/
	std::cout<<(std::chrono::system_clock::now()-start).count()<<std::endl;

	/*for(auto it : v) {
		std::cout<<it<<' ';
	}
	std::cout<<std::endl;*/

	return 0;
}
