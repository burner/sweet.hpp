// use this done to disable the logger at compile time
//#define SWEET_LOGGER_RELEASE
//
#include <logger.hpp>

int main() {
	LOG("here");  					// this will always log
	LOG(128, "args"); 				// logger with numbers have to enabled first
	SweetLogger::enableLogger(128); // like this
	LOG(128, "args"); 				// than they will print to std::cerr
	return 0;
}
