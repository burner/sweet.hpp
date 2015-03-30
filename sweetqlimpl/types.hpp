#pragma once

#include <vector>
#include <string>
#include <ostream>
#include <type_traits>

enum class SweetqlFlags {
	PrimaryKey = 1
};

enum class SweetqlTypes {
	Int,
	Float,
	String,
	Blob
};

inline std::ostream& operator<<(std::ostream& out, const SweetqlTypes& t) {
	switch(t) {
		case SweetqlTypes::Int: out<<"Int"; break;
		case SweetqlTypes::Float: out<<"Float"; break;
		case SweetqlTypes::String: out<<"String"; break;
		case SweetqlTypes::Blob: out<<"Blob"; break;
		default: out<<"Undefined"; break;
	}

	return out;
}

typedef void(*del)(void*);

template<typename T>
class SqlAttribute {
public:
	inline SqlAttribute(SweetqlTypes t) : type(t) {}
	inline SqlAttribute(SweetqlTypes t, SweetqlFlags p) : type(t) {
		primaryKey = p;
	}

	inline virtual int64_t getInt(T&) const { 
		throw std::logic_error("getInt not implemented"); 
	}
	inline virtual double getFloat(T&) const { 
		throw std::logic_error("getFloat not implemented"); 
	}
	inline virtual std::string getString(T&) const { 
		throw std::logic_error("getString not implemented"); 
	}
	inline virtual void* getBlob(T&) const { 
		throw std::logic_error("getBlob not implemented"); 
	}
	inline virtual size_t getBlobSize(T&) const { 
		throw std::logic_error("getBlobSize not implemented"); 
	}
	inline virtual del getBlobDel(T&) const { 
		throw std::logic_error("getBlobSize not implemented"); 
	}
	inline virtual void setInt(T&,int64_t) {
		throw std::logic_error("setInt not implemented"); 
	}
	inline virtual void setFloat(T&,double) {
		throw std::logic_error("setFloat not implemented"); 
	}
	inline virtual void setString(T&,const std::string&) {
		throw std::logic_error("setString not implemented"); 
	}
	inline virtual void setString(T&,const char*) {
		throw std::logic_error("setString not implemented"); 
	}
	inline virtual void setBlob(T&,const void*,size_t) {
		throw std::logic_error("setBlob not implemented"); 
	}

	inline std::string getType() const {
		switch(this->type) {
		case SweetqlTypes::String: return "varchar";
		case SweetqlTypes::Int: return "integer";
		case SweetqlTypes::Float: return "real";
		default: 
			throw std::logic_error(std::string("wroong type for getType"));
		}
	}

	SweetqlFlags primaryKey;
	SweetqlTypes type;
};

inline bool isPrimaryKey(SweetqlFlags flags) {
	return static_cast<int>(flags) & 
		static_cast<int>(SweetqlFlags::PrimaryKey);
}

template<typename T>
class SqlStringAttribute : public SqlAttribute<T> {
public:
	inline SqlStringAttribute(std::string T::* s) : SqlAttribute<T>(SweetqlTypes::String), 
		str(s) {}

	inline SqlStringAttribute(std::string T::* s, SweetqlFlags f) : 
		SqlAttribute<T>(SweetqlTypes::String, f), str(s) {}

	inline std::string getString(T& t) const { 
		return t.*str;
	}
	inline void setString(T& t,const std::string& s) {
		t.*str = s;
	}
	inline void setString(T& t,const char* s) {
		t.*str = s;
	}
private:
	std::string T::* str;
};

template<typename T>
class SqlIntAttribute : public SqlAttribute<T> {
public:
	inline SqlIntAttribute(int64_t T::* i) : SqlAttribute<T>(SweetqlTypes::Int), integer(i) {}
	inline SqlIntAttribute(int64_t T::* i, SweetqlFlags f) : 
		SqlAttribute<T>(SweetqlTypes::Int, f), integer(i) {}

	inline int64_t getInt(T& t) const { 
		return t.*integer;
	}
	inline void setInt(T& t, int64_t i) {
		t.*integer = i;
	}
private:
	int64_t T::* integer;
};

template<typename T>
class SqlFloatAttribute : public SqlAttribute<T> {
public:
	inline SqlFloatAttribute(double T::* i) : SqlAttribute<T>(SweetqlTypes::Float), fl(i) {}
	inline SqlFloatAttribute(double T::* i, SweetqlFlags f) : 
		SqlAttribute<T>(SweetqlTypes::Float, f), fl(i) {}

	inline double getFloat(T& t) const { 
		return t.*fl;
	}
	inline void setFloat(T& t, double i) {
		t.*fl = i;
	}
private:
	double T::* fl;
};

template<typename S, typename T>
std::shared_ptr<SqlAttribute<T>> makeAttr(S T::* i,
		typename std::enable_if<std::is_integral<S>::value, S>::type = 0) {
	return std::make_shared<SqlIntAttribute<T>>(i);
}

template<typename S, typename T>
std::shared_ptr<SqlAttribute<T>> makeAttr(S T::* i,
		typename std::enable_if<std::is_floating_point<S>::value, S>::type = 0) {
	return std::make_shared<SqlFloatAttribute<T>>(i);
}

template<typename S, typename T>
std::shared_ptr<SqlAttribute<T>> makeAttr(S T::* i,
		typename std::enable_if<std::is_same<S,std::string>::value, S>::type = "") {
	return std::make_shared<SqlStringAttribute<T>>(i);
}

template<typename S, typename T>
std::shared_ptr<SqlAttribute<T>> makeAttr(S T::* i,
		size_t /*size*/, del /*d*/) {
	return std::make_shared<SqlStringAttribute<T>>(i);
}

// With flags

template<typename S, typename T>
std::shared_ptr<SqlAttribute<T>> makeAttr(S T::* i, SweetqlFlags f,
		typename std::enable_if<std::is_integral<S>::value, S>::type = 0) {
	return std::make_shared<SqlIntAttribute<T>>(i, f);
}

template<typename S, typename T>
std::shared_ptr<SqlAttribute<T>> makeAttr(S T::* i, SweetqlFlags f,
		typename std::enable_if<std::is_floating_point<S>::value, S>::type = 0) {
	return std::make_shared<SqlFloatAttribute<T>>(i, f);
}

template<typename S, typename T>
std::shared_ptr<SqlAttribute<T>> makeAttr(S T::* i, SweetqlFlags f,
		typename std::enable_if<std::is_same<S,std::string>::value, S>::type = "") {
	return std::make_shared<SqlStringAttribute<T>>(i, f);
}

template<typename S, typename T>
std::shared_ptr<SqlAttribute<T>> makeAttr(S T::* i, SweetqlFlags f,
		size_t /*size*/, del /*d*/) {
	return std::make_shared<SqlStringAttribute<T>>(i, f);
}

template<typename T>
class SqlColumn {
public:
	inline SqlColumn(const std::string& a, std::shared_ptr<SqlAttribute<T>> at) : 
			attrName(a), attr(at) {
	}

	std::string attrName;
	std::shared_ptr<SqlAttribute<T>> attr;
};

template<typename T>
class SqlTable {
public:
	template<typename S>
	static void storeColumn(SqlTable& t, S col) {
		t.column.push_back(col);
	}

	template<typename S, typename... Col>
	static void storeColumn(SqlTable& t, S col, Col... c) {
		t.column.push_back(col);
		storeColumn(t,c...);
	}

	template<typename... Col>
	static SqlTable<T> sqlTable(const std::string& n, Col... cs) {
		SqlTable<T> tab;
		tab.name = n;
		storeColumn(tab, cs...);
		return tab;
	}

	std::string name;
	std::vector<SqlColumn<T>> column;
};
