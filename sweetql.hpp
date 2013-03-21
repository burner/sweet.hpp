#ifndef SWEETQL_HPP
#define SWEETQL_HPP
#include <functional>

class SqlObj {
public:
	virtual void initSqlObj() = 0;
	template<typename T, typename S, typename R>
	void linkName(const std::string& name, 
			S get, R set) {

	}
};

template<typename T>
class dbIter {
public:
	dbIter() {}

private:
	T it;

};

template<typename... T>
class SqliteDB {
public:
	SqliteDB(const std::string& dbfn) {
	}

	template<typename S>
	bool insert(S& t) {
		return false;
	}

	template<typename S>
	std::pair<dbIter<S>,dbIter<S>> select(const std::string& where = "") {
		return std::make_pair<dbIter<S>,dbIter<S>>(dbIter<S>(), dbIter<S>());
	}
};
#endif
