#ifndef SWEE_TCPPJSON_HPP
#define SWEE_TCPPJSON_HPP

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

//namespace sjson {
class value;
typedef std::shared_ptr<value> ValuePtr;
typedef std::vector<std::shared_ptr<value>> ValuePtrVec;
class object;
typedef std::shared_ptr<object> ObjectPtr;
//}

//namespace sjson {

static bool isString(const std::string& toTest) {
	return toTest.size() >= 1;
}

/*UNITTEST(isStringTest) {
	AS_T(isString("h"));
	AS_T(isString("hello"));
	AS_F(isString(""));
}*/

static bool convertsToFloat(const std::string& s, double& ret) {
	try { ret = std::stof(s); }
	catch(...) { return false; }
	return true;
}

static bool convertsToInt(const std::string& s, long& ret) {
	try { ret = std::stol(s); } 
	catch(...) { return false; }
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
			long i;
			double d;
			if(convertsToInt(s, i)) setType(value::type_number_int); 
			else if(convertsToFloat(s, d)) setType(value::type_number_float); 
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
	inline ValuePtrVec getArray() { return array; }
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
	inline void print(std::ostream&, size_t);
};

class object {
private:
	std::unordered_map<std::string,ValuePtr> mappings;
public:
	/** This function returns the mapping contained by the object. */
	inline std::unordered_map<std::string,ValuePtr>& getMappings() { 
		return mappings; 
	}

	/** Given a dot separated path this function returns a corresponding
	 * value. */
	inline ValuePtr access(const std::string& path) {
		size_t pos = path.find('.');
		auto pathSubString(path.substr(0,pos));
		auto ret = this->mappings.find(pathSubString);
		if(ret == this->mappings.end()) {
			throw std::logic_error(std::string("Path not present ") + path);
		} else if(pos <= path.size()) {
			if(ret->second->getType() != value::type_object) {
				throw std::logic_error(std::string("Path ") + path + 
					std::string(" did not lead to an object"));
			} else {
				return ret->second->getObject()->access(
					path.substr(pos+1,path.size())
				);
			}
		} else {
			return ret->second;
		}
	}

	inline bool pathExists(const std::string& path) {
		try { auto ret = this->access(path); }
		catch(...) { return false; }
		return true;
	}
	void print(std::ostream&, size_t);
};

class jsonparser {
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
	/** Returns the root of the parsed json tree.
	 *
	 * @return The root object of the tree.
	 */
	inline ObjectPtr getRoot() { return root; }

	/** The passed vector can contain the tokens for parsing
	jsonparser(std::vector<std::string>);*/

	/** This methode starts the parsing process. */
	inline void parse() { readline(); root = parseObject(); }

	inline bool testAndEatOrThrow(const char c, bool nothrow = false) {
		if(curLine[idx] != c) { 
			if(!nothrow) throw std::logic_error(locToStr() + std::string(
				" excepeted a ") + c + " but found a " + curLine[idx] + " at " +
				locToStr());
			else return false; }
		else { ++idx; return true; }
	}

	inline std::string parseString() {
		size_t e = curLine.find('"', idx);
		if(e == std::string::npos) { 
			throw std::logic_error(locToStr() + "excepted and '\"'"); }
		else { 
			std::string ret = curLine.substr(idx, e-idx-1); 
			idx += e + 1; 
			return ret;
		}
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
		testAndEatOrThrow('{');

		do {
			eatWhitespace();
			testAndEatOrThrow('"');
			std::string name(parseString());
			eatWhitespace();
			testAndEatOrThrow(':');
			eatWhitespace();
			ValuePtr vp = parseValue();
			obj->getMappings().insert(std::make_pair(name, vp));
			eatWhitespace();
			testAndEatOrThrow(',', true);

		} while(curLine[idx] != '}');
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
		if(testAndEatOrThrow('{', true)) {
			ValuePtr ret(std::make_shared<value>());
			ret->setObject(parseObject());
			ret->setType(value::type_object);
			return ret;
		} else if(testAndEatOrThrow('[', true)) {
			ValuePtr ret(std::make_shared<value>());
			ret->setArray(parseArray());
			ret->setType(value::type_array);
			return ret;
		} else if(testAndEatOrThrow('"', true)) {
			ValuePtr ret(std::make_shared<value>(parseString()));
		} 
		throw std::logic_error(locToStr() + "no value found");
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
			ret.push_back(parseValue());
			eatWhitespace();
			if(curLine[idx] == ',') {
				++idx;
				continue;
			}
		} while(curLine[idx] != ']');
		return ret;
	}

	/** Returns the current Token.
	 *
	 * @return The current Token.
	inline const std::string& curToken() const;
	 */

	/** As a stream and the parsed json file will be pretty printed to it.
	 *
	 * @param The stream to print to.
	 */
	inline void print(std::ostream&);
	//const std::string& nextToken();

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
		if(idx >= curLine.size()) { readline(); }
		while(!eof && (curLine[idx] == ' ' || curLine[idx] == '\t')) {
			++idx;
			if(idx >= curLine.size()) { readline(); }
		}
	}
};

/** Returns a jsonparser who has parsed the file given by the parameter.
jsonparser parseFile(const std::string&);

jsonparser parseString(const std::string&);

std::vector<std::string> getJsonTokenFormFile(const std::string& filename);
*/

//} // cppjson
#endif
