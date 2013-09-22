#include <string>
#include <sstream>
#include <fstream>
#include <boost/regex.hpp>
#include <utility>
#include <iostream>
#include <algorithm>
#include <sweetql.hpp>
//#include <conv.hpp>
#include <benchmark.hpp>

    /*class Person {
    public:
    	Person() {} // dummy
    
    	static SqlTable<Person>& table() {
    		static SqlTable<Person> tab = SqlTable<Person>::sqlTable( 
    			"Person",
    			SqlColumn<Reservation>("Firstname", makeAttr(&Reservation::firstname)),
    			SqlColumn<Reservation>("Lastname", 	makeAttr(&Reservation::lastname)),
    			SqlColumn<Reservation>("Age", 		makeAttr(&Reservation::age)),
    		return tab;
    	}
	private:
    	std::string firstname;
    	std::string lastname;
    	int age;
    };*/

class Person {
public:
	Person() {} // dummy
	Person(const std::string& f, const std::string& l, const std::string& c, 
			const std::string& a, const std::string& a2,
			const std::string& ci, const std::string& s, int z, 
			const std::string& pw, const std::string& pp, const std::string& m, 
			const std::string& w) :
		firstname(f),	 lastname(l),	 company(c),
 		address(a),		 county(a2),	 city(ci),
 		state(s),		 phoneWork(pw),	 phonePrivat(pp),
 		mail(m),		 www(w),		 zip(z) {
	}

	std::string firstname;
	std::string lastname;
	std::string company;
	std::string address;
	std::string county;
	std::string city;
	std::string state;
	std::string phoneWork;
	std::string phonePrivat;
	std::string mail;
	std::string www;
	int zip;

	static SqlTable<Person>& table() {
		static SqlTable<Person> tab = SqlTable<Person>::sqlTable("Person",
			SqlColumn<Person>("Firstname", 	makeAttr(&Person::firstname)),
			SqlColumn<Person>("Lastname", 	makeAttr(&Person::lastname)),
			SqlColumn<Person>("Company", 	makeAttr(&Person::company)),
			SqlColumn<Person>("Address", 	makeAttr(&Person::address)),
			SqlColumn<Person>("County", 	makeAttr(&Person::county)),
			SqlColumn<Person>("City", 		makeAttr(&Person::city)),
			SqlColumn<Person>("State", 		makeAttr(&Person::state)),
			SqlColumn<Person>("PhoneWork", 	makeAttr(&Person::phoneWork)),
			SqlColumn<Person>("PhonePrivat",makeAttr(&Person::phonePrivat)),
			SqlColumn<Person>("Mail", 		makeAttr(&Person::mail)),
			SqlColumn<Person>("Www", 		makeAttr(&Person::www)),
			SqlColumn<Person>("Zip", 		makeAttr(&Person::zip))
		);
		return tab;
	}
};

typedef std::vector<Person> PersonVec;

class ReservationPerson {
public:
	ReservationPerson() {}

	static SqlTable<ReservationPerson>& table() {
		static SqlTable<ReservationPerson> tab = 
			SqlTable<ReservationPerson>::sqlTable("ReservationPerson",
		SqlColumn<ReservationPerson>("Firstname", makeAttr(&ReservationPerson::firstname)),
		SqlColumn<ReservationPerson>("Lastname", makeAttr(&ReservationPerson::lastname)),
		SqlColumn<ReservationPerson>("Location", makeAttr(&ReservationPerson::location)),
		SqlColumn<ReservationPerson>("Date", makeAttr(&ReservationPerson::date)),
		SqlColumn<ReservationPerson>("PhonePrivat", makeAttr(&ReservationPerson::phonePrivat))
		);
		return tab;
	}

	std::string firstname;
	std::string lastname;
	std::string location;
	std::string date;
	std::string phonePrivat;
};

PersonVec parsePersonFile(const std::string& fn) {
	PersonVec ret;
	std::ifstream infile(fn);
	std::vector<std::string> v;
	std::string line;
	boost::regex re("\"[^\"]+\"");
	while(std::getline(infile, line)) {
		auto reBe = boost::sregex_iterator(line.begin(), line.end(), re);
    	auto reEn = boost::sregex_iterator();
		std::transform(reBe, reEn, std::back_inserter(v), [&v]
			(const boost::smatch& it) {
				return it.str().substr(1, it.str().size()-2);
			}
		);
		ret.push_back(Person(v[0], v[1], v[2], v[3], v[4], v[5], v[6],
					stoi(v[7]), v[8], v[9], v[10], v[11]));
		v.clear();
	}

	return ret;
}

int main() {
	SqliteDB db("testtable.db");

	Bench in;
	std::vector<Person> per = parsePersonFile("50000.csv");
	in.stop();
	std::cout<<"Reading the file took "<<in.milli()<<" msec"<<std::endl;

	Bench insert;
	db.insert<Person>(per.begin(), per.end());
	//Reservation a("Danny", "Zeckzer", "Armsen", "02.04.2013");
	//db.insert<Reservation>(a);
	insert.stop();
	std::cout<<"Writting the persons to the db took "<<insert.milli()
		<<" msec"<<std::endl;

	Bench s;
	auto sel(db.select<Person>("Firstname=\"Danny\" and Lastname=\"Zeckzer\""));
	Person toDel;
	//auto sel(db.select<Person>());
	/*std::for_each(sel.first, sel.second, [&toDel](const Person& p) {
		std::cout<<p.firstname<<' ';
		std::cout<<p.lastname<<' ';
		std::cout<<p.company<<' ';
		std::cout<<p.address<<' ';
		std::cout<<p.county<<' ';
		std::cout<<p.zip<<' ';
		std::cout<<p.state<<' ';
		std::cout<<p.phoneWork<<' ';
		std::cout<<p.phonePrivat<<' ';
		std::cout<<p.mail<<' ';
		std::cout<<p.www;
		std::cout<<std::endl;
		toDel = p;
	});*/
	
	s.stop();

	/*auto rpSel(db.join<ReservationPerson, Person, Reservation>());
	std::for_each(rpSel.first, rpSel.second, [](const ReservationPerson& rp) {
		std::cout<<rp.firstname<<' ';
		std::cout<<rp.lastname<<' ';
		std::cout<<rp.location<<' ';
		std::cout<<rp.date<<' ';
		std::cout<<rp.phonePrivat<<std::endl;
	});

	std::cout<<"Selecting persons from the db took "<<s.micro()
		<<" microsec"<<std::endl;
	
	std::cout<<toDel.firstname<<' ';
	std::cout<<toDel.lastname<<' ';
	std::cout<<toDel.company<<' ';
	std::cout<<toDel.address<<' ';
	std::cout<<toDel.county<<' ';
	std::cout<<toDel.zip<<' ';
	std::cout<<toDel.state<<' ';
	std::cout<<toDel.phoneWork<<' ';
	std::cout<<toDel.phonePrivat<<' ';
	std::cout<<toDel.mail<<' ';
	std::cout<<toDel.www<<std::endl;

	db.remove(toDel);
	sel = db.select<Person>("Firstname=\"Danny\" and Lastname=\"Zeckzer\"");
	std::for_each(sel.first, sel.second, [](const Person& p) {
		std::cout<<p.firstname<<' ';
		std::cout<<p.lastname<<' ';
		std::cout<<p.company<<' ';
		std::cout<<p.address<<' ';
		std::cout<<p.county<<' ';
		std::cout<<p.zip<<' ';
		std::cout<<p.state<<' ';
		std::cout<<p.phoneWork<<' ';
		std::cout<<p.phonePrivat<<' ';
		std::cout<<p.mail<<' ';
		std::cout<<p.www<<std::endl;
	});*/
}
