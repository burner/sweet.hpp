#include <location.hpp>
#include <format.hpp>

Loc::Loc() : line(0), column(0) {}

Loc::Loc(unsigned l, unsigned c) : line(l), column(c) {
	
}

Loc::Loc(std::string f, unsigned l, unsigned c) : file(f), line(l), 
		column(c) {

}

std::string Loc::toString() const {
	if(file.size() == 0u) {	
		return format("%u:%u",line,column);
	} else {
		return format("%s:%u:%u",file,line,column);
	}
}
