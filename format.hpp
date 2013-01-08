#ifndef SWEET_FORMAT
#define SWEET_FORMAT
#include <stdexcept>
#include <algorithm>
#include <ostream>
#include <sstream>
#include <locale>
#include <iomanip>
#include <type_traits>

static inline void setStreamFormat(std::ostream& out, const std::string& form, size_t s,
		size_t e) {
	// check the flags
	if(form[s] == '-') {
		out<<std::left;
		++s;	
	} else if(form[s] == '+') {
		out<<std::right;
		++s;	
	} else if(form[s] == '0') {
		out<<std::setfill('0');
		++s;
	} else if(form[s] == '#') {
		out<<std::showbase;
		++s;
	}

	// if we find no dot only the width will follow	
	size_t dot = form.substr(s, e-s).find(".");
	if(dot != std::string::npos) {
		dot += s;
	}

	std::string width(form.substr(s, dot < e ? dot-s : e-s));
	if(!std::all_of(width.begin(), width.end(), [](const char c) {
			return c <= '9' && c >= '0';})) {
		throw std::invalid_argument("width string did not convert to integer");
	} else if(!width.empty()) {
		out<<std::setw(std::stoi(width));
	}
	s = dot < form.size() ? dot+1 : form.size();
	if(s < e) {
		std::string pre(form.substr(s, e-s));
		if(!std::all_of(pre.begin(), pre.end(), [](const char c) {
				return c <= '9' && c >= '0';})) {
			throw std::invalid_argument("precision string did not convert to integer");
		} else if(!pre.empty()) {
			out<<std::setprecision(std::stoi(pre));
		}
		s = e;
	}

	// lower or upper case
	out<<(std::islower(form[s]) ? std::nouppercase : std::uppercase);
	if(form[s] == 'f' || form[s] == 'F') {
		out<<std::fixed;
	} else if(form[s] == 'a' || form[s] == 'A' || form[s] == 'x' || 
			form[s] == 'X' || form[s] == 'p') {
		out<<std::hex;
	} else if(form[s] == 'e' || form[s] == 'E') {
		out<<std::scientific;
	} else if(form[s] == 'o') {
		out<<std::oct;
	}
}

static inline void formImpl(std::ostream& out, const std::string& s, size_t pos) {
    while(pos != s.size()) {
        if(s[pos] == '%') {
            if(s[pos + 1] == '%') {
                ++pos;
            } else {
                throw std::runtime_error("invalid format string: missing arguments");
            }
        }
        out<<s[pos++];
    }
}
 
template<typename T, typename... Args>
static inline void formImpl(std::ostream& out, const std::string& s, size_t pos, T value, Args... args) {
    while(pos != s.size()) {
        if(s[pos] == '%') {
            if(s[pos + 1] == '%') {
                ++pos;
			} else {
				auto savedFlags(out.flags());
				size_t next(s.find_first_of("csdioxXufFeEaAgGp", pos+1));
				setStreamFormat(out, s, pos+1, next);
				if(s[next] == 'p') {
					//printPointer(out, value);
					out<<*(reinterpret_cast<const void**>(&value));
				} else {
					out<<value;
				}
				out.flags(savedFlags);
				pos = next;
                formImpl(out, s, pos+1, args...); // call even when *s == 0 to detect extra arguments
                return;
            }
        }
        out<< s[pos++];
    }
    throw std::logic_error("extra arguments provided to format");
}

template<typename... Args>
inline std::string format(const std::string& str, Args... args) {
	std::stringstream ret;
	formImpl(ret, str,0, args...);
	return ret.str();
}

template<typename... Args>
inline void formatToStream(std::ostream& out, const std::string& str, Args... args) {
	formImpl(out, str,0, args...);
}
#endif
