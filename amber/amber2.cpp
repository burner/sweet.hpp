#include "amber3.hpp"

#include <fstream>

int main(int argc, char *argv[]) {
	std::ofstream out("amber2.html");		

	int a;
	fun(out, a);
	return 0;
}
