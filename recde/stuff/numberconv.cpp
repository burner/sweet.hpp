#include <numberconv.hpp>
#include <exception>
#include <typeinfo>
#include <limits.h>
#include <unit.hpp>
#include <format.hpp>

class FormatException : public std::exception {
	private:
		std::string text;

	public:
		FormatException(const std::string& txt) : text(txt) {};

		virtual const char* what() const throw() {
			return text.c_str();
		}
}; 

template <typename Type>
bool valueTest(const Type value) {
	if(value < std::numeric_limits<Type>::min() || 
			value > std::numeric_limits<Type>::max()) {
		return false;
	} else {
		return true;
	}
}

long charToBase10(const char in) {
	switch(in) {
		case '0' : return 0;
		case '1' : return 1;
		case '2' : return 2;
		case '3' : return 3;
		case '4' : return 4;
		case '5' : return 5;
		case '6' : return 6;
		case '7' : return 7;
		case '8' : return 8;
		case '9' : return 9;
		default:
			throw FormatException(
				format("%s:%d cannot convert '%c' to digit int value %d",
				__FILE__,__LINE__,in,static_cast<int>(in))
			);
	}
}

const bool isDigit(const char in) {
	if(in >= '0' && in <= '9') {
		return true;
	} else {
		return false;
	}
}

template<typename Ret> 
Ret stringToIntegerImpl(const std::string& str, int base) {
	bool neg = false;
	long tmp = 0;
	long mul = 1;
	long ret = 0;
	auto it = str.rbegin();

	for(; it != str.rend(); it++) {
		if(*it == '_' || *it == 'b' || *it == 's' || *it == 'l' 
				|| *it == 'u' || *it == ' ') {
			continue;
		} else if(*it == '-') {
			neg = true;
			continue;
		} else if(*it == '+') {
			neg = false;
			continue;
		}

		//std::cout<<__LINE__<<" "<<*it<<std::endl;
		tmp = charToBase10(*it) * mul;
		ret += tmp;
		//std::cout<<__LINE__<<" "<<ret<<std::endl;
		mul *= base;
	}

	if(!valueTest<Ret>(ret)) {
		throw FormatException(
			format("%s:%d cannot convert %d to char",__FILE__,__LINE__,ret)
		);
	} else {
		return static_cast<Ret>(neg ? - ret : ret);
	}
}

char stringToChar(const std::string& str, int base) {
	return stringToIntegerImpl<char>(str,base);
}

unsigned char stringToUChar(const std::string& str, int base) {
	return stringToIntegerImpl<unsigned char>(str,base);
}

short stringToShort(const std::string& str, int base) {
	return stringToIntegerImpl<short>(str,base);
}

unsigned short stringToUShort(const std::string& str, int base) {
	return stringToIntegerImpl<unsigned short>(str,base);
}

int stringToInt(const std::string& str, int base) {
	return stringToIntegerImpl<int>(str,base);
}

unsigned int stringToUInt(const std::string& str, int base) {
	return stringToIntegerImpl<unsigned int>(str,base);
}

long stringToLong(const std::string& str, int base) {
	return stringToIntegerImpl<long>(str,base);
}

unsigned long stringToULong(const std::string& str, int base) {
	return stringToIntegerImpl<unsigned long>(str,base);
}

float stringToFloat(const std::string& str) {
	std::stringstream ss(str);
	float ret;
	ss >> ret;
	return ret;
}

double stringToDouble(const std::string& str) {
	std::stringstream ss(str);
	double ret;
	ss >> ret;
	return ret;
}

long double stringToReal(const std::string& str) {
	std::stringstream ss(str);
	long double ret;
	ss >> ret;
	return ret;
}

/*UNITTEST(charToBase10Test) {
	AS_EQ(charToBase10('0') ,0);
	AS_EQ(charToBase10('1') ,1);
	AS_EQ(charToBase10('2') ,2);
	AS_EQ(charToBase10('3') ,3);
	AS_EQ(charToBase10('4') ,4);
	AS_EQ(charToBase10('5') ,5);
	AS_EQ(charToBase10('6') ,6);
	AS_EQ(charToBase10('7') ,7);
	AS_EQ(charToBase10('8') ,8);
	AS_EQ(charToBase10('9') ,9);

	bool cought = false;
	try {
		charToBase10('a');
	} catch(FormatException e) {
		cought = true;
	} 
	AS_T(cought);
}

UNITTEST(isDigitTest) {
	AS_T(isDigit('0'));
	AS_T(isDigit('1'));
	AS_T(isDigit('2'));
	AS_T(isDigit('3'));
	AS_T(isDigit('4'));
	AS_T(isDigit('5'));
	AS_T(isDigit('6'));
	AS_T(isDigit('7'));
	AS_T(isDigit('8'));
	AS_T(isDigit('9'));
	AS_T(!isDigit('r'));
}

UNITTEST(stringToIntTest) {
	AS_EQ(stringToInt("1337"), 1337);
	AS_EQ(stringToInt("1_337"), 1337);
	AS_EQ(stringToInt("1__3_3_7_"), 1337);
}

UNITTEST(stringToFloatTest) {
	AS_EQ(stringToDouble("1337.0"), 1337.0);
	AS_EQ(stringToDouble("1.0E2"), 100.0);
	AS_EQ(stringToDouble("1.0E-1"), 0.1);
	AS_EQ(stringToDouble("5.0"), 5.0);
	AS_EQ(stringToDouble("1.0E2"), 100.0);
	AS_EQ(stringToDouble("1.0E-1"), 0.1);
}*/
