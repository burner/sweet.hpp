#pragma once

#include <mysql/mysql.h>

class MySQL {
public:
	inline MySQL(const std::string& u, const std::string& un, 
			const std::string& pwd, const std::string& db, const uint32_t p,
			const std::string& unixs, const uint32_t f)
		: url(u), username(un), password(pwd), database(db), port(p),
			unixsocket(unixs), flag(f)
	{
		this->con = mysql_init(nullptr);
		if(this->con == nullptr) {
			throw std::logic_error("MySQL init failed");
		}

		if(mysql_real_connect(con, this->url.c_str(), this->url.c_str(),
				this->password.c_str(), this->database.c_str(), this->port,
				this->unixsocket.c_str(), this->flag))
		{
			throw std::logic_error("MySQL real_connect failed");
		}

	}

	std::string url;
	std::string username;
	std::string password;
	std::string database;
	uint32_t port;
	std::string unixsocket;
	uint32_t flag;
private:
	MYSQL* con;
};
