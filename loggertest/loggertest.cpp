// use this done to disable the logger at compile time
//#define SWEET_LOGGER_RELEASE
//
#include <logger.hpp>
#include <unit.hpp>

UNITTEST(loggertest_test1) {
	LOG("here");  					// this will always log
	LOG(128, "args"); 				// logger with numbers have to enabled first
	sweet::enableLogger(128); // like this
	LOG(128, "args"); 				// than they will print to std::cerr
}
