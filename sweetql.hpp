#ifndef SWEETQL_HPP
#define SWEETQL_HPP
#include <algorithm>
#include <functional>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iterator>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

// yes I know
#pragma GCC diagnostic ignored "-Wpmf-conversions"
#pragma GCC diagnostic ignored "-Wpointer-arith"

class SqlColumn {
public:
	/*SqlColumn(const std::string& a, void* s, void* g) : attr(a),
		set(s), get(g) {
	}*/

	SqlColumn(const std::string& a, void* s, void* g, size_t t) : attr(a),
		set(s), get(g), type(t) {
	}

	template<typename D, typename S, typename G>
	static SqlColumn sqlColumn(const std::string& a, S s, G g) {
		return SqlColumn(a, 
			reinterpret_cast<void*>(s),
			reinterpret_cast<void*>(g),
			typeid(D).hash_code()
		);
				
	}
	std::string attr;
	void* set;
	void* get;
	size_t type;
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


class SqliteDB {
public:
	template<typename T>
	class Iterator {
	public:
		Iterator() {}
		typedef Iterator self_type;
		typedef T value_type;
		typedef T& reference;
		typedef T* pointer;
		typedef std::forward_iterator_tag iterator_category;
		typedef int difference_type;
		self_type operator++() { }
		self_type operator++(int) { }
		reference operator*() { return it; }
		pointer operator->() { return &it; }
		bool operator==(const self_type& rhs) { return false; }
		bool operator!=(const self_type& rhs) { return false; }
	
	private:
		T it;
	
	};

	SqliteDB(const std::string& dbn) : dbName(dbn) {
		int rc = sqlite3_open(dbName.c_str(), &db);
		if(rc) {
			throw std::logic_error(std::string("Can't open database ") +
					dbName + " because of " + sqlite3_errmsg(db));
		}
	}

	~SqliteDB() {
		sqlite3_close(db);
	}

	template<typename S>
	bool insert(S& t) {
		SqlTable& tab = S::table();
		std::stringstream stmtStr;
		stmtStr<<"INSERT INTO ";
		stmtStr<<tab.name<<"(";
		std::transform(tab.column.begin(), tab.column.end(),
				std::ostream_iterator<std::string>(stmtStr, ","), 
				[](const SqlColumn& c) {
					return c.attr;
				}
		);
		stmtStr<<"\b) VALUES(";
		std::for_each(tab.column.begin(), tab.column.end(), [&t,&stmtStr]
			(const SqlColumn& c) {
				auto tmp = reinterpret_cast<const std::string&(*)(S*)>(c.get);
				if(c.type == typeid(int).hash_code() || c.type ==
					typeid(float).hash_code()) {
					stmtStr<<(*tmp)(&t)<<", ";
				} else {
					stmtStr<<'\''<<(*tmp)(&t)<<"\', ";
				}
			}
		);
		stmtStr<<"\b\b);";
		std::cout<<stmtStr.str()<<std::endl;

		if(sqlite3_exec(db, "INSERT INTO Person(Firstname,Lastname) VALUES('Oliver', 'Theel');", 
				0, 0, 0) != SQLITE_OK) {
			throw std::logic_error(std::string("Insert Statment:\"") +
					stmtStr.str() + "\" failed with error:\"" +
					sqlite3_errmsg(db) + "\"");
		}

		return false;
	}

	template<typename S>
	std::pair<SqliteDB::Iterator<S>,SqliteDB::Iterator<S>> select(const std::string& where = "") {
		return std::make_pair<SqliteDB::Iterator<S>,SqliteDB::Iterator<S>>(SqliteDB::Iterator<S>(),
				SqliteDB::Iterator<S>());
	}

private:
	std::string dbName;
	sqlite3 *db;
};
#endif
