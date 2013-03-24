#include <string>
#include <utility>
#include <iostream>
#include <algorithm>
#include "sweetql.hpp"
#include "conv.hpp"

class Person {
public:
	Person() {} // dummy
	Person(const std::string& f, const std::string& l, unsigned a, 
			const std::string& o) : age(a), firstname(f), lastname(l), occupation(o) {
	}

	const std::string& getAge() {
		std::stringstream ret;
		ret<<age;
		age_s = ret.str();
		return age_s;
	}

	const std::string& getFirstname() const {
		return firstname;
	}

	const std::string& getLastname() const {
		return lastname;
	}

	const std::string& getOccupation() const {
		return occupation;
	}

	void setAge(const std::string& a) {
		age = to<unsigned>(a);
	}

	void setFirstname(const std::string& f) {
		firstname = f;
	}

	void setLastname(const std::string& l) {
		lastname = l;
	}

	void setOccupation(const std::string& o) {
		occupation = o;
	}

	static SqlTable& table() {
		static SqlTable tab = SqlTable::sqlTable("Person",
			SqlColumn::sqlColumn("age", &Person::getAge, &Person::setAge),
			SqlColumn::sqlColumn("firstname", &Person::getFirstname, &Person::setFirstname));
		return tab;
	}

private:
	unsigned age;
	std::string age_s;
	std::string firstname;
	std::string lastname;
	std::string occupation;
};


int main() {
	SqliteDB db("person.sql"
	);

	Person me("Robert", "Schadek", 26, "Assistent");
	db.insert(me);

	std::pair<dbIter<Person>,dbIter<Person>> sel;
	sel = db.select<Person>("firstname == Robert and age > 20");
	/*std::for_each(sel.first, sel.second, [](const Person& p) {
		std::cout<<p.lastname()<<std::endl;
	});*/
}
