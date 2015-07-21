// use this done to disable the logger at compile time
//#define SWEET_LOGGER_RELEASE
//
#include <log.hpp>
#include <fstream>
#include <unit.hpp>

UNITTEST(loggertest) {
	auto logOut = std::make_unique<std::ofstream>("logFile.log", 
	//auto logOut = std::make_unique<std::ofstream>("/home/burner/storage/logFile.log", 
		std::ios::app);
	sweet::getLoggerDrain().setOutput(std::move(logOut));
	sweet::getLoggerDrain().alwaysFlush = true;

	std::thread arr[4];
	for(int i = 0; i < 4; ++i) {
		arr[i] = std::thread([&](int j) {
			for(size_t i = 0; i < 100000; ++i) {
				sweet::LOG(1)("/home/burner/storage/logFile.log") << " " <<j;
				sweet::LOG(128)( "args") << " " <<j;
				sweet::LOG(128)("args")<<" "<<"hello"<<std::endl<<"some more "<<j<<std::endl;
			}
		}, i);
	}

	for(int i = 0; i < 4; ++i) {
		arr[i].join();	
	}
}
