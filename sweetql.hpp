// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
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

struct SqlAttribute {
	const std::string& operator()() const {
		return this->data;
	}

	void operator()(const std::string& s) {
		this->data = s;
	}

	private:
		std::string data;
};

template<typename T>
class SqlColumn {
public:
	SqlColumn(const std::string& a, SqlAttribute T::* at) : attrName(a),
		attr(at) {
	}

	std::string attrName;
	SqlAttribute T::* attr;
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

class SqliteDB {
public:
	template<typename T>
	class Iterator {
	public:
		typedef Iterator self_type;
		typedef T value_type;
		typedef T& reference;
		typedef T* pointer;
		typedef std::forward_iterator_tag iterator_category;
		typedef int difference_type;

		Iterator() : sqlRsltCode(SQLITE_DONE) {}

		Iterator(int rsc, sqlite3_stmt* s) : sqlRsltCode(rsc), stmt(s) {
			if(sqlRsltCode == SQLITE_OK) {
				sqlRsltCode = sqlite3_step(stmt);
				if(sqlRsltCode == SQLITE_ROW) {
					buildObj();
				} else {
					sqlite3_finalize(stmt);
				}
			} else {
				sqlite3_finalize(stmt);
			}
		}

		inline void buildObj() {
			int cc = sqlite3_column_count(stmt);
			auto tab = T::table();
			for(int i = 0; i < cc; ++i) {
				std::string cn = reinterpret_cast<const char*>
					(sqlite3_column_name(stmt, i));
				for(auto cm : tab.column) {
					if(cn == cm.attrName) {
						(it.*(cm.attr))(reinterpret_cast<const char*>(
							sqlite3_column_text(stmt, i)));
						break;
					}
				}
			}
		}

		self_type operator++() { 
			self_type copy = *this;
			if(sqlRsltCode == SQLITE_ROW) {
				sqlRsltCode = sqlite3_step(stmt);
				if(sqlRsltCode == SQLITE_ROW && sqlRsltCode != SQLITE_ERROR 
						&& sqlRsltCode != SQLITE_DONE) {
					buildObj();
				}
				return copy;
			} else {
				sqlite3_finalize(stmt);
				return Iterator();
			}
		}

		reference operator*() { return it; }
		pointer operator->() { return &it; }

		bool operator==(const self_type& rhs) { 
			if(sqlRsltCode != rhs.sqlRsltCode) { 
				return false;
			} else if(sqlRsltCode == rhs.sqlRsltCode 
					&& sqlRsltCode == SQLITE_ROW) {
				int tCnt = sqlite3_column_count(stmt);
				int oCnt = sqlite3_column_count(rhs.stmt);

				if(tCnt != oCnt) {
					return false;
				}

				// if the column entries are different the it is different
				for(int it = 0; it < tCnt; ++it) {
					std::string tc = reinterpret_cast<const char*>(
						sqlite3_column_text(stmt, it)
					);
					std::string oc = reinterpret_cast<const char*>(
						sqlite3_column_text(rhs.stmt, it)
					);
					if(tc != oc) {
						return false;
					}
				}
				return true;
			} else if(sqlRsltCode == rhs.sqlRsltCode) {
				return true;
			} else {
				return false;
			}
		}

		bool operator!=(const self_type& rhs) { return !(*this == rhs); }
	
	private:
		T it;
		int sqlRsltCode;
		sqlite3_stmt* stmt;
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
	inline static const std::string prepareStatment() {
		const SqlTable<S>& tab = S::table();
		std::stringstream stmtStr;
		stmtStr<<"INSERT INTO ";
		stmtStr<<tab.name<<"(";
		size_t numColumn = tab.column.size()-1;
		for(size_t i = 0; i < numColumn; ++i) {
			stmtStr<<tab.column[i].attrName<<',';
		}
		stmtStr<<tab.column[numColumn].attrName<<") Values(";
		size_t i = 1;
		for(; i <= numColumn; ++i) {
			stmtStr<<'?'<<i<<',';
		}
		stmtStr<<'?'<<i<<");";
		return stmtStr.str();
	}

	inline void step(sqlite3_stmt* stmt, const std::string& stmtStr) {
		if(sqlite3_step(stmt) != SQLITE_DONE) {
			throw std::logic_error(std::string("Insert Statment:\"") +
					stmtStr + "\" failed with error:\"" +
					sqlite3_errmsg(db) + "\"");
		}
	}

	template<typename S>
	inline bool insert(S& t) {
		SqlTable<S>& tab = S::table();
		const std::string stmtStr(prepareStatment<S>());
		sqlite3_stmt* stmt;
		sqlite3_prepare_v2(db, stmtStr.c_str(), stmtStr.size(), &stmt, NULL);
		addParameter(t, tab, stmt);
		step(stmt, stmtStr);

		return true;
	}

	template<typename S, typename It>
	inline bool insert(It be, It en) {
		SqlTable<S>& tab = S::table();
		const std::string stmtStr(prepareStatment<S>());
		sqlite3_stmt* stmt;
		char* errorMessage;
		sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
		sqlite3_prepare_v2(db, stmtStr.c_str(), stmtStr.size(), &stmt, NULL);
		std::for_each(be, en, [this,&stmt,&stmtStr,&tab] (S& it) {
			addParameter(it,tab, stmt);						
			step(stmt, stmtStr);
			sqlite3_reset(stmt);
		});
		sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
		if(sqlite3_finalize(stmt) != SQLITE_OK) {
			throw std::logic_error(std::string("Insert Statment:\"") +
					stmtStr + "\" failed with error:\"" +
					sqlite3_errmsg(db) + "\"");
		}

		return true;
	}

	template<typename S>
	std::pair<SqliteDB::Iterator<S>,SqliteDB::Iterator<S>> select(
			const std::string& where = "") {
		std::stringstream stmtStr;
		stmtStr<<"SELECT * FROM "<<S::table().name;
		if(!where.empty()) {
			stmtStr<<" WHERE "<<where;
		}
		stmtStr<<';';
		return makeIterator<S>(stmtStr.str());
	}

	template<typename S>
	void remove(S& s) {
		SqlTable<S> keys = keyColumn<S>();
		sqlite3_stmt* stmt;
		std::stringstream stmtStr;
		stmtStr<<"DELETE FROM "<<S::table().name<<" WHERE ";
		const size_t size = keys.column.size()-1;
		for(size_t i = 0; i < size; ++i) {
			stmtStr<<keys.column[i].attrName<<"=? and ";
		}
		stmtStr<<keys.column[size].attrName<<"=?";
		sqlite3_prepare_v2(db, stmtStr.str().c_str(), stmtStr.str().size(), 
			&stmt, NULL);
		addParameter(s, keys, stmt);
		step(stmt, stmtStr.str());
		sqlite3_finalize(stmt);
	}

	template<typename S, typename T, typename R>
	std::pair<SqliteDB::Iterator<S>,SqliteDB::Iterator<S>> join(
			const std::string& where = "") {
		std::stringstream stmtStr;
		stmtStr<<"SELECT * FROM "<<T::table().name<<" NATURAL JOIN "<<
			R::table().name;
		if(!where.empty()) {
			stmtStr<<" WHERE "<<where;
		}
		stmtStr<<';';
		return makeIterator<S>(stmtStr.str());
	}

private:
	template<typename S>
	inline std::pair<SqliteDB::Iterator<S>,SqliteDB::Iterator<S>> makeIterator(
			const std::string stmtStr) {
		sqlite3_stmt* stmt;
 		int rsltCode = sqlite3_prepare(db, stmtStr.c_str(), -1, 
			&stmt, NULL
		);
		if(rsltCode == SQLITE_ERROR) {
			throw std::logic_error(std::string("Select Statment:\"") +
					stmtStr + "\" failed with error:\"" +
					sqlite3_errmsg(db) + "\"");
		}
		return std::make_pair(
				SqliteDB::Iterator<S>(rsltCode, stmt),
				SqliteDB::Iterator<S>());

	}
	
	template<typename S>
	inline static void addParameter(S& t, SqlTable<S>& tab, 
			sqlite3_stmt* stmt) {
		int i = 1;
		std::for_each(tab.column.begin(), tab.column.end(), [&t,&i,&stmt]
			(const SqlColumn<S>& c) {
				const std::string& s = (t.*(c.attr))();
				sqlite3_bind_text(stmt, i++, s.c_str(), s.size(),
					SQLITE_STATIC);
			}
		);

	}

	template<typename S>
	inline SqlTable<S> keyColumn() {
		SqlTable<S> ret;
		SqlTable<S>& tab = S::table();
		std::for_each(tab.column.begin(), tab.column.end(), [this,&ret,&tab]
				(const SqlColumn<S>& c) {
			char const * dataType;
			char const * seqN;
			int nN = 0;
			int pK = 0;
			int aI = 0;
			sqlite3_table_column_metadata(db,NULL,tab.name.c_str(),
				c.attrName.c_str(), &dataType, &seqN, &nN, &pK, &aI);
			if(pK) {
				ret.column.push_back(c);
			}
		});
		return ret;
	}

	std::string dbName;
	sqlite3 *db;

};
#endif
