#include <options.hpp>

struct Config {
	int value = 0;
};

int main(int argc, char** argv) {
	Config config;
	bool b = "false";
	int c;
	std::vector<int> fs;
	sweet::Options opt(argc, argv);
	opt.get("-bt", "--bool", "test if bool were set alot more bool"
			"more boring test sjfalaskjdflkjsdfl kjasldfkjalsdkfj "
			" asldfjasldf jalsdfkj ksjdfl jlksjdfl kjljfdsaf", b);
	opt.get("-c", "--intlong", "test if int were set", config.value);
	opt.getMultiple("-f", "--funs", "test if multiple int were set", fs);
	opt.finalize();
	std::cout<<__LINE__<<" "<<b<<" "<<config.value<<std::endl;
	for(auto f : fs) {
		std::cout<<f<<std::endl;
	}
}
