#include <fstream>
#include <string>
#include <cerrno>
#include <stdexcept>

#include <options.hpp>
#include <format.hpp>

#include <rulestore.hpp>
#include <ruleparser.hpp>

std::string getFileContents(const std::string& filename) {
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if(in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	throw std::logic_error(format("Failed to read file with name '%s'", filename));
}

int main(int argc, char** argv) {
	std::string inputFile;
	Options opts(argc, argv);
	opts.get("-i", "--inputFile", "The grammar file to parse", inputFile);

	RuleStore store;
	RuleParser parser(getFileContents(inputFile), store.token, store.rules);
	return 0;
}
