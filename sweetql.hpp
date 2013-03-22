#ifndef SWEETQL_HPP
#define SWEETQL_HPP
#include <algorithm>
#include <functional>
#include <vector>
#include <stdexcept>

// yes I know
#pragma GCC diagnostic ignored "-Wpmf-conversions"

class SqlColumn {
public:
	template<typename S,typename G>
	SqlColumn(const std::string& a, S s, G g) : attr(a),
	set(reinterpret_cast<void*>(s)), get(reinterpret_cast<void*>(g)) {
	}
private:
	std::string attr;
	void* set;
	void* get;
};

class SqlTable {
public:
	template<typename... Col>
	SqlTable(const std::string& n, Col... cs) : name(n) {
	}

	template<typename T, typename... Col>
	void storeColumn(T col, Col... c) {
		column.push_back(col);
		storeColumn(c...);
	}

	template<typename T>
	void storeColumn(T col) {
		column.push_back(col);
	}

	std::string name;
private:
	std::vector<SqlColumn> column;
};

template<typename T>
class dbIter {
public:
	dbIter() {}

private:
	T it;

};

class SqliteDB {
public:
	template<typename ...Table>
	SqliteDB(const std::string& dbfn, Table... ts) {
		storeTable(ts...);
		std::sort(functions.begin(), functions.end(), [](
				const SqlTable& a, const SqlTable& b) {
			return a.name < b.name;
		});
	}

	template<typename T, typename... Table>
	void storeTable(T table, Table... tables) {
		functions.push_back(table);
		storeTable(tables...);
	}

	template<typename T>
	void storeTable(T table) {
		functions.push_back(table);
	}

	template<typename S>
	bool insert(S& t) {
		return false;
	}

	template<typename S>
	std::pair<dbIter<S>,dbIter<S>> select(const std::string& where = "") {
		return std::make_pair<dbIter<S>,dbIter<S>>(dbIter<S>(), dbIter<S>());
	}

private:
	std::vector<SqlTable> functions;
};
#endif
