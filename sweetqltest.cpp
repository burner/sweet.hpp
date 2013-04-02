#include <string>
#include <sstream>
#include <fstream>
#include <boost/regex.hpp>
#include <utility>
#include <iostream>
#include <algorithm>
#include "sweetql.hpp"
#include "conv.hpp"
#include "benchmark.hpp"

class Reservation {
public:
	Reservation() {} // dummy
	Reservation(const std::string& f, const std::string& l, 
			const std::string& lo, const std::string& d) { 
		firstname(f);
		lastname(l);
 		location(lo);
 		date(d);
	}

	static SqlTable<Reservation>& table() {
		static SqlTable<Reservation> tab = SqlTable<Reservation>::sqlTable( 
			"Reservation",
			SqlColumn<Reservation>("Firstname", &Reservation::firstname),
			SqlColumn<Reservation>("Lastname", &Reservation::lastname),
			SqlColumn<Reservation>("Location", &Reservation::location),
			SqlColumn<Reservation>("Date", &Reservation::date));
		return tab;
	}

	SqlAttribute firstname;
	SqlAttribute lastname;
	SqlAttribute location;
	SqlAttribute date;
};

class Person {
public:
	Person() {} // dummy
	Person(const std::string& f, const std::string& l, const std::string& c, 
			const std::string& a, const std::string& a2,
			const std::string& ci, const std::string& s, const std::string& z, 
			const std::string& pw, const std::string& pp, const std::string& m, 
			const std::string& w) {
		firstname(f);	 lastname(l);	 company(c);
 		address(a);		 county(a2);	 city(ci);
 		state(s);		 phoneWork(pw);	 phonePrivat(pp);
 		mail(m);		 www(w);		 zip(z);
	}

	static SqlTable<Person>& table() {
		static SqlTable<Person> tab = SqlTable<Person>::sqlTable( "Person",
		SqlColumn<Person>("Firstname", &Person::firstname),
		SqlColumn<Person>("Lastname", &Person::lastname),
		SqlColumn<Person>("Company", &Person::company),
		SqlColumn<Person>("Address", &Person::address),
		SqlColumn<Person>("County", &Person::county),
		SqlColumn<Person>("City", &Person::city),
		SqlColumn<Person>("State", &Person::state),
		SqlColumn<Person>("Zip", &Person::zip),
		SqlColumn<Person>("PhoneWork", &Person::phoneWork),
		SqlColumn<Person>("PhonePrivat", &Person::phonePrivat),
		SqlColumn<Person>("Mail", &Person::mail),
		SqlColumn<Person>("Www", &Person::www));
		return tab;
	}

//private:
	SqlAttribute firstname;
	SqlAttribute lastname;
	SqlAttribute company;
	SqlAttribute address;
	SqlAttribute county;
	SqlAttribute city;
	SqlAttribute state;
	SqlAttribute zip;
	SqlAttribute phoneWork;
	SqlAttribute phonePrivat;
	SqlAttribute mail;
	SqlAttribute www;
};

typedef std::vector<Person> PersonVec;

class ReservationPerson {
public:
	ReservationPerson() {}

	static SqlTable<ReservationPerson>& table() {
		static SqlTable<ReservationPerson> tab = 
			SqlTable<ReservationPerson>::sqlTable(
		"ReservationPerson",
		SqlColumn<ReservationPerson>("Firstname",
			&ReservationPerson::firstname),
		SqlColumn<ReservationPerson>("Lastname", &ReservationPerson::lastname),
		SqlColumn<ReservationPerson>("Location", &ReservationPerson::location),
		SqlColumn<ReservationPerson>("Date", &ReservationPerson::date),
		SqlColumn<ReservationPerson>("PhonePrivat", 
			&ReservationPerson::phonePrivat)
		);
		return tab;
	}

	SqlAttribute firstname;
	SqlAttribute lastname;
	SqlAttribute location;
	SqlAttribute date;
	SqlAttribute phonePrivat;
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
		ret.push_back(Person(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7],
					v[8], v[9], v[10], v[11]));
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
	Reservation a("Danny", "Zeckzer", "Armsen", "02.04.2013");
	db.insert<Reservation>(a);
	insert.stop();
	std::cout<<"Writting the persons to the db took "<<insert.milli()
		<<" msec"<<std::endl;

	Bench s;
	Person toDel;
	auto sel(db.select<Person>("Firstname=\"Danny\" and Lastname=\"Zeckzer\""));
	//auto sel(db.select<Person>());
	std::for_each(sel.first, sel.second, [&toDel](const Person& p) {
		std::cout<<p.firstname()<<' ';
		std::cout<<p.lastname()<<' ';
		std::cout<<p.company()<<' ';
		std::cout<<p.address()<<' ';
		std::cout<<p.county()<<' ';
		std::cout<<p.zip()<<' ';
		std::cout<<p.state()<<' ';
		std::cout<<p.phoneWork()<<' ';
		std::cout<<p.phonePrivat()<<' ';
		std::cout<<p.mail()<<' ';
		std::cout<<p.www()<<std::endl;
		toDel = p;
	});
	s.stop();

	auto rpSel(db.join<ReservationPerson, Person, Reservation>());
	std::for_each(rpSel.first, rpSel.second, [](const ReservationPerson& rp) {
		std::cout<<rp.firstname()<<' ';
		std::cout<<rp.lastname()<<' ';
		std::cout<<rp.location()<<' ';
		std::cout<<rp.date()<<' ';
		std::cout<<rp.phonePrivat()<<std::endl;
	});

	std::cout<<"Selecting persons from the db took "<<s.micro()
		<<" microsec"<<std::endl;
	
	std::cout<<toDel.firstname()<<' ';
	std::cout<<toDel.lastname()<<' ';
	std::cout<<toDel.company()<<' ';
	std::cout<<toDel.address()<<' ';
	std::cout<<toDel.county()<<' ';
	std::cout<<toDel.zip()<<' ';
	std::cout<<toDel.state()<<' ';
	std::cout<<toDel.phoneWork()<<' ';
	std::cout<<toDel.phonePrivat()<<' ';
	std::cout<<toDel.mail()<<' ';
	std::cout<<toDel.www()<<std::endl;

	db.remove(toDel);
	sel = db.select<Person>("Firstname=\"Danny\" and Lastname=\"Zeckzer\"");
	std::for_each(sel.first, sel.second, [](const Person& p) {
		std::cout<<p.firstname()<<' ';
		std::cout<<p.lastname()<<' ';
		std::cout<<p.company()<<' ';
		std::cout<<p.address()<<' ';
		std::cout<<p.county()<<' ';
		std::cout<<p.zip()<<' ';
		std::cout<<p.state()<<' ';
		std::cout<<p.phoneWork()<<' ';
		std::cout<<p.phonePrivat()<<' ';
		std::cout<<p.mail()<<' ';
		std::cout<<p.www()<<std::endl;
	});
}
