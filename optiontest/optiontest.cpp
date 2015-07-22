#include <options.hpp>
#include <unit.hpp>

struct Config {
	int value = 0;
};

UNITTEST(optiontest) {
	const char* argv[14] = {"./optiontest", "--test", "1", "-b",
	   	"true", "-c", "3", "-f", "0", "--funs", "1", "-f", "2", "-h"};
	int argc = 13;	
	Config config;
	bool b = "false";
	int c;
	std::vector<int> fs;
	sweet::Options opt(argc, const_cast<char**>(argv));
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
