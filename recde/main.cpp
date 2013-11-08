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
	std::string outAstH("ast.hpp");
	std::string outAstS("ast.cpp");
	std::string outPrsH("parser.hpp");
	std::string outPrsS("parser.cpp");
	std::string outErrH("error.hpp");
	std::string outErrS("error.cpp");
	std::string outPrefix;
	Options opts(argc, argv);
	opts.get("-i", "--inputFile", "The grammar file to parse", inputFile);
	opts.get("-ah", "--astoutputheader", "Output file for ast header", outAstH);
	opts.get("-as", "--astoutputsource", "Output file for ast source", outAstS);
	opts.get("-ph", "--parseroutputheader", "Output file for parser header", outAstH);
	opts.get("-ps", "--parseroutputsource", "Output file for parser source", outAstS);
	opts.get("-eh", "--erroroutputheader", "Output file for error header", outAstH);
	opts.get("-es", "--erroroutputsource", "Output file for error source", outAstS);
	opts.get("-op", "--outputprefix", "All output files will be prefixed with this string", outPrefix);
	opts.finalize();

	if(!outPrefix.empty()) {
		outAstH = outPrefix + outAstH;
		outAstS = outPrefix + outAstS;
		outPrsH = outPrefix + outPrsH;
		outPrsS = outPrefix + outPrsS;
		outErrH = outPrefix + outErrH;
		outErrS = outPrefix + outErrS;
	}

	if(inputFile.empty()) {
		return 0;
	}

	RuleStore store;
	RuleParser parser(inputFile, store);
	parser.parse();
	std::ofstream astH(outAstH);
	std::ofstream astS(outAstS);
	std::ofstream prsH(outPrsH);
	std::ofstream prsS(outPrsS);
	std::ofstream errH(outErrH);
	std::ofstream errS(outErrS);
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
