#ifndef SWEETQL_HPP
#define SWEETQL_HPP
#include <algorithm>
#include <functional>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iterator>

// yes I know
#pragma GCC diagnostic ignored "-Wpmf-conversions"

class SqlColumn {
public:
	/*SqlColumn(const std::string& a, void* s, void* g) : attr(a),
		set(s), get(g) {
	}*/

	SqlColumn(const std::string& a, void** s) : attr(a), set(s) {
	}

	template<typename S>
	static SqlColumn sqlColumn(const std::string& a, S s) {
		return SqlColumn(a, reinterpret_cast<std::string**>(&s));
				
	}
	/*static SqlColumn sqlColumn(const std::string& a, S s, G g) {
		return SqlColumn(a, reinterpret_cast<void*>(s),
				reinterpret_cast<void*>(g));
	}*/
	std::string attr;
	void** set;
	//void* get;
};

class SqlTable {
public:
	template<typename T>
	static void storeColumn(SqlTable& t, T col) {
		t.column.push_back(col);
	}

	template<typename T, typename... Col>
	static void storeColumn(SqlTable& t, T col, Col... c) {
		t.column.push_back(col);
		storeColumn(t,c...);
	}

	template<typename... Col>
	static SqlTable sqlTable(const std::string& n, Col... cs) {
		SqlTable tab;
		tab.name = n;
		storeColumn(tab, cs...);
		return tab;
	}

	std::string name;
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
	//template<typename ...Table>
	SqliteDB(const std::string& dbfn) {
	}
	/*SqliteDB(const std::string& dbfn, Table... ts) {
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
	}*/

	template<typename S>
	bool insert(S& t) {
		SqlTable& tab = S::table();
		std::stringstream stmtStr("INSERT INTO ");
		stmtStr<<tab.name<<" (";
		std::transform(tab.column.begin(), tab.column.end(),
				std::ostream_iterator<std::string>(stmtStr, ","), 
				[](const SqlColumn& c) {
					return c.attr;
				}
		);
		stmtStr<<"\b) VALUES(";
		std::for_each(tab.column.begin(), tab.column.end(), [&stmtStr]
			(const SqlColumn& c) {
				auto tmp = *reinterpret_cast<std::string*>(c.set);
				//auto get = reinterpret_cast<const std::string&(S::*)()>(tmp);
			}
		);
		std::cout<<stmtStr.str()<<std::endl;
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
