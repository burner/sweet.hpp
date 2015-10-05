#include "amber2.hpp"
#include <fstream>

int main(int argc, char *argv[]) {
	std::ofstream o("amber2.html");	
	amber2(o, argc);
	return 0;
}
