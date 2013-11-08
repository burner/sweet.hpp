#include <string>
#include <sstream>
#include <stdlib.h>

bool isDigit(const char in);

char stringToChar(const std::string& str, int base = 10);
unsigned char stringToUChar(const std::string& str, int base = 10);

short stringToShort(const std::string& str, int base = 10);
unsigned short stringToUShort(const std::string& str, int base = 10);

int stringToInt(const std::string& str, int base = 10);
unsigned int stringToUInt(const std::string& str, int base = 10);

long stringToLong(const std::string& str, int base = 10);
unsigned long stringToULong(const std::string& str, int base = 10);

float stringToFloat(const std::string&);
double stringToDouble(const std::string&);
long double stringToReal(const std::string&);

template<typename T>
std::string toString(T input) {
	std::stringstream ss;
	ss<<input;
	return ss.str();
}
