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
	std::string outVH("visitor.hpp");
	std::string outovH("outvisitor.hpp");
	std::string outovS("outvisitor.cpp");
	std::string outdvH("dotvisitor.hpp");
	std::string outdvS("dotvisitor.cpp");
	std::string outMvH("multivisitor.hpp");
	std::string outMvS("multivisitor.cpp");
	std::string outH("visitorinclude");
	std::string outPrefix;
	Options opts(argc, argv);
	opts.get("-i", "--inputFile", "The grammar file to parse", inputFile);
	opts.get("-ah", "--astoutputheader", "Output file for ast header", outAstH);
	opts.get("-as", "--astoutputsource", "Output file for ast source", outAstS);
	opts.get("-ph", "--parseroutputheader", "Output file for parser header", 
		outPrsH);
	opts.get("-ps", "--parseroutputsource", "Output file for parser source", 
		outPrsS);
	opts.get("-eh", "--erroroutputheader", "Output file for error header", 
		outErrH);
	opts.get("-es", "--erroroutputsource", "Output file for error source", 
		outErrS);
	opts.get("-vh", "--visitoroutputheader", 
		"Output file for tree visitor header", outVH);
	opts.get("-oh", "--outvisitoroutputheader", 
		"Output file for std::cout visitor header", outovH);
	opts.get("-os", "--outvisitoroutputsource", 
		"Output file for std::cout visitor source", outovS);
	opts.get("-dh", "--dotvisitoroutputheader", 
		"Output file for Dot file visitor header", outdvH);
	opts.get("-ds", "--dotvisitoroutputsource", 
		"Output file for Dot file visitor source", outdvS);
	opts.get("-mh", "--multivisitoroutputheader", 
		"Output file for Multi visitor header", outMvH);
	opts.get("-ms", "--multivisitoroutputsource", 
		"Output file for Multi visitor source", outMvS);
	opts.get("-op", "--outputprefix", 
		"All output files will be prefixed with this string", outPrefix);
	opts.get("-in", "--visitorinclude", 
		"Visitor method declarations", outH);
	opts.finalize();

	if(!outPrefix.empty()) {
		outAstH = outPrefix + outAstH;
		outAstS = outPrefix + outAstS;
		outPrsH = outPrefix + outPrsH;
		outPrsS = outPrefix + outPrsS;
		outErrH = outPrefix + outErrH;
		outErrS = outPrefix + outErrS;
		outVH = outPrefix + outVH;
		outovH = outPrefix + outovH;
		outovS = outPrefix + outovS;
		outdvH = outPrefix + outdvH;
		outdvS = outPrefix + outdvS;
		outMvH = outPrefix + outMvH;
		outMvS = outPrefix + outMvS;
		outH = outPrefix + outH;
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
	std::ofstream visH(outVH);
	std::ofstream ovisH(outovH);
	std::ofstream ovisS(outovS);
	std::ofstream dvisH(outdvH);
	std::ofstream dvisS(outdvS);
	std::ofstream mvisH(outMvH);
	std::ofstream mvisS(outMvS);
	std::ofstream inH(outH);
	Output out(prsS,prsH,astS,astH,errS,errH,visH,ovisH,ovisS,dvisH,dvisS,
		mvisH,mvisS,inH
	);
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
	SweetLogger::enableLogger(64);
	rd.gen();
	LOG("%s\n%s\n", outPrsH, outPrsS);
	return 0;
}
