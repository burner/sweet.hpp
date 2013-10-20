#include <fstream>
#include <string>
#include <cerrno>
#include <stdexcept>

#include <options.hpp>
#include <format.hpp>

#include <rulestore.hpp>
#include <ruleparser.hpp>
#include <token.hpp>
#include <recdec.hpp>

int main(int argc, char** argv) {
	std::string inputFile;
	Options opts(argc, argv);
	opts.get("-i", "--inputFile", "The grammar file to parse", inputFile);
	opts.finalize();

	RuleStore store;
	RuleParser parser(inputFile, store);
	parser.parse();
	RecurDec rd(store, std::cout, std::cout);
	rd.computeFirstSet();
	//std::cout<<store.first<<std::endl;

	for(auto& it : store.token) {
		format(std::cout, "%s\n", it.second);
	}
	for(auto& it : store.rules) {
		format(std::cout, "%s : %s\n", it.first, it.second);
	}
	std::cout<<std::endl;
	rd.genRules();
	return 0;
}
