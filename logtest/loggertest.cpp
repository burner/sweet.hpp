// use this done to disable the logger at compile time
//#define SWEET_LOGGER_RELEASE
//
#include <log.hpp>
#include <fstream>

int main() {
	auto logOut = std::make_unique<std::ofstream>("logFile.log", 
	//auto logOut = std::make_unique<std::ofstream>("/home/burner/storage/logFile.log", 
		std::ios::app);
	sweet::getLoggerDrain().setOutput(std::move(logOut));
	sweet::getLoggerDrain().alwaysFlush = true;

	for(size_t i = 0; i < 100000; ++i) {
		sweet::LOG(1)("/home/burner/storage/logFile.log");
		sweet::LOG(128)( "args");
		sweet::LOG(128)("args")<<" "<<"hello"<<std::endl<<"some more"<<std::endl;
	}
	return 0;
}
