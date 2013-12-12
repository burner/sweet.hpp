#pragma once

#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm> 
#include <functional> 
#include <stdexcept> 
#include <unordered_map>
#include <ostream>
#include <vector>
#include <assert.h>
#include <logger.hpp>

namespace sweet {

class value;
typedef std::shared_ptr<value> ValuePtr;
typedef std::vector<std::shared_ptr<value>> ValuePtrVec;
class object;
typedef std::shared_ptr<object> ObjectPtr;

void print(std::ostream&, ValuePtr, size_t, bool);
void print(std::ostream&, ObjectPtr, size_t, bool);

static bool isString(const std::string& toTest) {
	return toTest.size() >= 1;
}

static bool convertsToFloat(const std::string& s) {
	/*try { ret = std::stof(s); }
	catch(...) { return false; }
	return true;*/
	const size_t size = s.size();
	size_t i = 0;
	if(size > 0 && (s[0] == '+' || s[0] == '-')) {
		i = 1;
	}
	for(i = 0; i < size; ++i) {
		if(!isdigit(s[i])) {
			break;
		}
	}
	if(i == size) {
		return true;
	} else if(s[i] == '.') {
		++i;
		for(; i < size; ++i) {
			if(!isdigit(s[i])) {
				break;
			}
		}
	}

	if(i == size) {
		return true;
	} else if(s[i] == 'e' || s[i] == 'E') {
		++i;
		if(s[i] == '+' || s[i] == '-') {
			++i;
		}
		for(; i < size; ++i) {
			if(!isdigit(s[i])) {
				break;
			}
		}
	}

	return i == size;
}

static bool convertsToInt(const std::string& s) {
	/*try { ret = std::stol(s); } 
	catch(...) { return false; }
	return true;*/
	const size_t size = s.size();
	size_t i = 0;
	if(size > 0 && (s[0] == '+' || s[0] == '-')) {
		i = 1;
	}
	for(i = 0; i < size; ++i) {
		if(!isdigit(s[i])) {
			return false;
		}
	}
	return true;
}

class value {
public:
	enum types {
		type_string,
		type_number_int,
		type_number_float,
		type_boolean,
		type_array,
		type_object,
		type_null
	};

private:
	// a value is either a string which maps (string,number,bool) or
	// it is object represented by the obj shared_ptr. The last possiblity
	// is an array of value shared_ptr.
	std::string str;
	ObjectPtr obj;
	ValuePtrVec array;

	types type;

public:
	inline value() : type(type_null) {}
	inline value(const std::string& s) : str(s) {
		if(s == "true" || s == "false") {
			this->type = value::type_boolean;
		} else if(s == "null") {
			this->type = value::type_null;
		} else {
			//long i;
			//double d;
			if(convertsToInt(s)) setType(value::type_number_int); 
			else if(convertsToFloat(s)) setType(value::type_number_float); 
			else if(s == "null") this->setType(value::type_null);
			else if(isString(s)) this->setType(value::type_string);
			else {throw std::logic_error(std::string("unexcepted input while"
				"parsing value input was: ") + s);
			}
		}
	}

	/** Returns the type of the value
	 *
	 * @return The type of the value.
	 */
	inline types getType() const { return type; }
	inline void setType(types t) { type = t; }

	/** If the value holds a bool this function will return true or false. */
	inline bool getBool() const { return this->str == "true"; }

	/** If the value holds a integer this function will return the int value. */
	inline long getInt() const { return std::stol(this->str); }

	/** If the value holds a float this function will return the float value. */
	inline double getFloat() const { return std::stod(this->str); }

	/** If the value holds a string this function will return it. */
	inline const std::string& getString() const { return str; }
	inline void setString(const std::string& s) { str = s; }

	/** If the value holds an array this function will return it. */
	inline const ValuePtrVec& getArray() const { return array; }
	inline void setArray(ValuePtrVec v) { array = v; }

	/** If the value holds an object this function will return it. */
	inline ObjectPtr getObject() { 
		if(!this->obj || this->type != value::type_object) {
			throw std::logic_error("value is not of type object");
		}
		return this->obj;
	}

	inline void setObject(ObjectPtr o) { obj = o; }
	inline bool operator==(const value& v) const { 
		return str == v.getString(); }
	inline bool operator<(const value& v) const { return str < v.getString(); }

};

class object {
private:
	std::unordered_map<std::string,ValuePtr> mappings;

	inline ValuePtr accessImpl(const std::string& path, const bool th, 
			bool& rls) {
		size_t pos = path.find('.');
		auto pathSubString(path.substr(0,pos));
		auto ret = this->mappings.find(pathSubString);
		if(ret == this->mappings.end()) {
			if(th) {
				throw std::logic_error(std::string("Path not present ") + path);
			} else {
				rls = false;
				ValuePtr r;
				return r;
			}
		} else if(pos <= path.size()) {
			if(ret->second->getType() != value::type_object) {
				if(th) {
					throw std::logic_error(std::string("Path ") + path + 
						std::string(" did not lead to an object"));
				} else {
					rls = false;
					ValuePtr r;
					return r;
				}
			} else {
				return ret->second->getObject()->accessImpl(
					path.substr(pos+1,path.size()), th, rls
				);
			}
		} else {
			rls = true;
			return ret->second;
		}

	}

public:
	/** This function returns the mapping contained by the object. */
	inline std::unordered_map<std::string,ValuePtr>& getMappings() { 
		return mappings; 
	}

	/** Given a dot separated path this function returns a corresponding
	 * value. */
	inline ValuePtr access(const std::string& path) {
		bool dontCare;
		return accessImpl(path, true, dontCare);
	}

	inline bool pathExists(const std::string& path) {
		bool rslt;
		accessImpl(path, false, rslt);
		return rslt;
	}

	template<typename T>
	typename std::enable_if<std::is_integral<T>::value,T>::type conv(ValuePtr t)
	{
		if(t->getType() != value::type_number_int) { 
			throw std::logic_error("Value not of type int"); 
		}
		return t->getInt();
	}

	template<typename T>
	typename std::enable_if<std::is_floating_point<T>::value,T>::type 
			conv(ValuePtr t) 
	{
		if(t->getType() != value::type_number_float) { 
			throw std::logic_error("Value not of type float"); 
		}
		return t->getFloat();
	}

	template<typename T>
	typename std::enable_if<std::is_same<T,bool>::value,bool>::type 
			conv(ValuePtr t) {
		if(t->getType() != value::type_boolean) { 
			throw std::logic_error("Value not of type bool"); 
		}
		return t->getBool();
	}

	template<typename T>
	typename std::enable_if<std::is_same<T,std::string>::value,
			std::string>::type 
		conv(ValuePtr t) {
		return t->getString();
	}

	template<typename T>
	inline T get(const std::string& path, T notFound) {
		bool exis;
		auto rslt = accessImpl(path, false, exis);
		if(exis) { return conv<T>(rslt); }
		else { return notFound; }
	}
};

class jsonparser {
friend std::ostream& operator<<(std::ostream&, const jsonparser&);
private:
	ObjectPtr root;
	size_t line;
	size_t idx;
	bool eof;
	bool fileOrStr;
	std::ifstream inputISS;
	std::istringstream* strRef;
	std::string curLine;

public:
	inline std::string locToStr() const {
		std::stringstream ret;
		ret<<line<<':'<<idx;
		return ret.str();
	}

	inline jsonparser(const std::string& fn) : line(0), idx(0),
			fileOrStr(false), inputISS(fn), strRef(nullptr), curLine("") {
		parse(); 
	}
	inline jsonparser(std::istringstream& is) : line(0), idx(0),
			fileOrStr(true), strRef(&is), curLine("") { 
		parse(); 
	}

	inline ~jsonparser() {
		if(!fileOrStr) {
			inputISS.close();
		}
	}

	/** Returns the root of the parsed json tree.
	 *
	 * @return The root object of the tree.
	 */
	inline ObjectPtr getRoot() { return root; }
	inline ObjectPtr getRoot() const { return root; }

	/** The passed vector can contain the tokens for parsing
	jsonparser(std::vector<std::string>);*/

	/** This methode starts the parsing process. */
	inline void parse() { readline(); root = parseObject(); }

	inline bool testAndEatOrThrow(const char c, const std::string& par, 
			bool nothrow = false) {
		if(curLine[idx] != c) { 
			if(!nothrow) throw std::logic_error(locToStr() + std::string(
				" excepeted a ") + c + " but found a " + curLine[idx] + " at " +
				locToStr() + " while parsing a " + par);
			else return false; }
		else { ++idx; return true; }
	}

	inline bool stringBreak(const char c) {
		return c == ':' || c == '}' || c == '{' || c == '[' || c == ']' 
			|| c == ',';
	}

	inline std::string parseString() {
		std::stringstream ss;
		bool isTickString = curLine[idx] == '"';
		if(isTickString) { 
			++idx; 
		}

		for(; idx < curLine.size(); ++idx) {
			if(!isTickString && 
					(std::isspace(curLine[idx]) || stringBreak(curLine[idx]))) {
				break;
			} else if(isTickString && idx > 0 && curLine[idx] == '"' &&
					curLine[idx-1] != '\\' && !(idx+1 < curLine.size() && curLine[idx+1] == '\\')) {
				break;
			} else if(isTickString && idx+1 < curLine.size() && 
					curLine[idx] == '"' && curLine[idx+1] == '\\') {
				idx+=2;
				eatWhitespace();
				if(curLine[idx] != '"') {
					throw std::logic_error(locToStr() + std::string(
						" excepeted a \" but found a " + curLine[idx]
					));
				}
				continue;
			} 
			ss<<curLine[idx];
		}
		if(isTickString) {
			++idx;
		}
		return ss.str();
	}

	inline std::string parseNullOrString() {
		size_t f = idx;
		std::string ret;
		for(;f < curLine.size() && std::isalnum(curLine[f]); ++f) {
			//std::cout<<curLine[f]<<std::endl;
			ret.push_back(curLine[f]);
		}
		idx = f+1;
		return ret;
	}

	inline std::string parseNumber() {
		size_t f = curLine.find_first_not_of("0123456789.-+", idx);
		std::string ret = curLine.substr(idx, f-idx); 
		idx = f; 
		return ret;
	}

	/** This method parses an json object. The object needs to be enclosed by a 
	 * { * and a }.
	 *
	 * @return A shared_ptr which points to an object.
	 * @throws std::logic error if parsing fails.
	 */
	inline ObjectPtr parseObject() {
		ObjectPtr obj = std::make_shared<object>();	
		eatWhitespace();
		testAndEatOrThrow('{', "Object");

		do {
			eatWhitespace();
			//testAndEatOrThrow('"', "Object");
			std::string name(parseString());
			eatWhitespace();
			testAndEatOrThrow(':', "Object");
			eatWhitespace();
			ValuePtr vp = parseValue();
			obj->getMappings().insert(std::make_pair(name, vp));
			eatWhitespace();
			testAndEatOrThrow(',', "Object", true);
			eatWhitespace();
		} while(curLine[idx] != '}');
		++idx;
		eatWhitespace();

		return obj;
	}

	/** This method parses an json value. A value can be a string, a integer
	 * or float number, an object, an array or a "null" string.
	 *
	 * @return A shared_ptr which points to an value.
	 * @throws std::logic error if parsing fails.
	 */
	inline ValuePtr parseValue() {
		eatWhitespace();
		if(curLine[idx] == '{') {
			ValuePtr ret(std::make_shared<value>());
			ret->setObject(parseObject());
			ret->setType(value::type_object);
			return ret;
		} else if(testAndEatOrThrow('[', "Value", true)) {
			ValuePtr ret(std::make_shared<value>());
			ret->setArray(parseArray());
			ret->setType(value::type_array);
			return ret;
		} else if(testAndEatOrThrow('"', "Value", true)) {
			--idx;
			return std::make_shared<value>(parseString());
		} else if(isdigit(curLine[idx]) || curLine[idx] == '+' || 
				curLine[idx] == '-') {
			return std::make_shared<value>(parseNumber());
		} else if(curLine.find("false", idx) != std::string::npos) {
			idx+=5;
			return std::make_shared<value>("false");
		} else if(curLine.find("true", idx) != std::string::npos) {
			idx+=4;
			return std::make_shared<value>("true");
		} else if(testAndEatOrThrow(']', "Value", true)) {
			ValuePtr ret(std::make_shared<value>());
			ret->setType(value::type_object);
			return ret;
		//} else if(parseNullOrString()) {
		} else if(std::islower(curLine[idx]) || std::isupper(curLine[idx])) {
			std::string parsedString(parseNullOrString());
			if(parsedString == "null") {
				ValuePtr ret(std::make_shared<value>());
				ret->setType(value::type_null);
				return ret;
			} else {
				return std::make_shared<value>(parsedString);
			}
		}
		throw std::logic_error(locToStr() + " no value found curLine(" +
				curLine + ") curChar(" + curLine[idx] + ")");

	}

	/** Arrays consists of values or better said shared_ptr of values. The
	 * array is represented by a vector. An array is enclosed by a [ and a ].
	 *
	 * @return A vector of shared_ptr to values.
	 */
	inline ValuePtrVec parseArray() {
		ValuePtrVec ret;
		do {
			eatWhitespace();	
			if(curLine[idx] == ']') {
				++idx;
				return ret;
				}
				ret.push_back(parseValue());
			eatWhitespace();
			if(curLine[idx] == ',') {
				++idx;
				continue;
			}
		} while(curLine[idx] != ']');
		++idx;
		eatWhitespace();
		return ret;
	}

	/** Returns the current Token.
	 *
	 * @return The current Token.
	inline const std::string& curToken() const;
	 */

private:
	inline void readline() {
		if(!fileOrStr) {
			assert(inputISS.is_open());
			eof = !std::getline(inputISS,curLine);
		} else {
			eof = !std::getline(*strRef,curLine);
		}
		idx = 0;
		++line;
	}

	inline void eatWhitespace() {
		begin:
		if(idx >= curLine.size()) { readline(); }
		while(!eof && (curLine.size() == 0 
				 || curLine[idx] == ' ' 
				 || curLine[idx] == '\t')) {
			++idx;
			if(idx >= curLine.size()) { 
				readline(); 
			}
		}
		if(idx < curLine.size() && curLine[idx] == '#') {
			readline();
			goto begin;
		}
	}
};

inline void print(std::ostream& o, ValuePtr v, size_t tab, bool) {
	if(v->getType() == value::type_number_int 
			|| v->getType() == value::type_number_float 
			|| v->getType() == value::type_boolean 
			|| v->getType() == value::type_null) {
		o<<v->getString(); } 
	else if(v->getType() == value::type_string) { o<<'"'<<v->getString()<<'"'; }
	else if(v->getType() == value::type_array) { 
		o<<'['<<std::endl;
		auto it = v->getArray().begin();
		for(; it != v->getArray().end(); ++it) {
			print(o, *it, tab+1, false);
			if(it+1 != v->getArray().end()) { o<<','; }
			o<<std::endl;
		}
		o<<']'<<std::endl; }
	else if(v->getType() == value::type_object) { 
		print(o, v->getObject(), tab+1, true); o<<std::endl; }
}

inline void print(std::ostream& o, ObjectPtr p, size_t tab, bool firstIn) {
	if(!firstIn) for(size_t i = 0; i < tab; ++i) { o<<"    "; }
	o<<'{'<<std::endl;
	auto it = p->getMappings().begin();
	for(; it != p->getMappings().end();) {
		for(size_t i = 0; i < tab+1; ++i) { o<<"    "; }
		o<<(*it).first<<" : "; print(o, (*it).second, tab+1, false);
		if((++it) != p->getMappings().end()) { o<<','; }
		o<<std::endl;
	}
	o<<'}'<<std::endl;
}

/** As a stream and the parsed json file will be pretty printed to it.
 *
 * @param The stream to print to.
 */
inline std::ostream& operator<<(std::ostream& o, const jsonparser& p) {
	print(o, p.getRoot(), 0, false);
	return o;
}

} // cppjson
