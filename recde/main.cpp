#include <fstream>
#include <string>
#include <cerrno>
#include <stdexcept>

#include <options.hpp>
#include <format.hpp>
#include <logger.hpp>

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
	std::ofstream astH("ast.hpp");
	std::ofstream astS("ast.cpp");
	std::ofstream prsH("parser.hpp");
	std::ofstream prsS("parser.cpp");
	std::ofstream errH("error.hpp");
	std::ofstream errS("error.cpp");
	Output out(prsS,prsH,astS,astH,errS,errH, "error.hpp");
	RecurDec rd(store, out);
	rd.computeFirstSet();
	//std::cout<<store.first<<std::endl;

	for(auto& it : store.token) {
		LOG("%s\n", it.second);
	}
	for(auto& it : store.rules) {
		LOG("%s : %s\n", it.first, it.second);
	}
	std::cout<<std::endl;
	rd.gen();
	return 0;
}
