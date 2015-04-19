SWEET_HPP
=========

A bunch of header useful for daily life. The headers contain functionally for
| Functionality            | Documentation | Header | Test |
| -------------------------|---------------|--------|------|
| Logging                  |               |        |      | 
| Asyc Logging             |               |        |      | 
| Unit testing             |               |        |      | 
| Design by Contract       |               |        |      | 
| Filesystem               |               |        |      | 
| Fixed-point Types        |               |        |      | 
| int128                   |               |        |      |
| Benchmarking             |               |        |      | 
| Json                     |               |        |      | 
| Trie                     |               |        |      |
| Base64                   |               |        |      |
| bitset                   |               |        |      |
| Allocator                |               |        |      |
| Safe Type Conversation   |               |        |      |
| Fixed Size Vector        |               |        |      |
| Fixed Size Map           |               |        |      |
| String formatting        |               |        |      |
| Cmd line parser          |               |        |      |
| Semaphore                |               |        |      |
| Stream helper functions  |               |        |      |
| String helper functions  |               |        |      |
| Sysinfo                  |               |        |      |

Usage
-----
To use the header, just make the sweet.hpp folder an include for your compiler.



unit.hpp
--------
Extremely small unit testing framework with a lot of nice unit testing
features. All assert macros also have a non unit test version usable in
"normal" code. Also includes QuickCheck like functionality.

```C++
UNITTEST(InformativTestName) {
	// your test here
	AS_T(boolValue);
	AS_T_M(boolValue, "some msg");
	AS_T_C(boolValue, [&]() { /* your code here */ });
	AS_EQ(1, 54);
	AS_NEQ(1, 54);
	AS_NEQ_C(1, 54, [&](/* your code here */ ) { });
}

int main() {
	if(!sweet::Unit::runTests()) {
		std::cerr<<"Some errors occoured"<<std::endl;
	}
	return 0;
}
```

More examples in: [here](unittest/unittest.cpp)

 format.hpp
------------

Variadic template type safe printf string formatting functions. Internally it
uses std::stream operation, so custom types can be used by creating custom
opertor<<(std::ostream&, CustomType); functions.

```C++
std::string s = format("Hello %s", "World");
format(someOutputStream, "Hello %s", "World");
```

More examples in: [here](formattest/formattest.cpp)

logger.hpp 
------------ 

Easy to use logger.

```C++
void someRandomFunction() {
	LOG();
	LOG("Hello");
	LOG("Hello %s", "World");
}
```

More examples in: [here](loggertest/loggertest.cpp)

dbc.hpp
---------

We have unittest maybe some Design by Contract might also help.

```C++
struct SomeStupidClass {
	int a = 0;
	int b = 9;

	INVARIANT_BEGIN
		Inv(RN(0,a,32));
		Inv(RN(0,b,10));
	INVARIANT_END

	inline void changeMethod() {
		Invariant(); // This makes the Invariant tested at the beginning and
					 //the end
		a = 33;		 	// this contradicts the invariant
	}
};

short testFunc(int a, double d, int* ip) {
	Rqr(RN(0,a,10), NaN(d), RN(0.0,d,1.0), NN(ip));
	// some cracy computation
	return Esr(RN(12, a +b *ip, 16));
}

void caller() {
	int a;
	int b = 9;
	short rslt = testFunc(9, 0.1, &a);
	SomeStupidClass ssc;
	ssc.changeMethod();
}
```

More examples in: [here](dbctest/dbctest.cpp)


benchmark.hpp
---------

Benchmark structs that limits typing work, a Bench struct that stops time and
a macro that stops time in a Compound Statement in a thread save manner.

```C++
#include <benchmark.hpp>

void fun() {
	BENCH(fun);
	...
}

int main() {
	sweet::Bench t; // Simple time stopping struct

	sweet::Benchmark::printResults(); // print results of BENCH marcos

	std::cout<<t.milli();
}
```

More examples in: [here](benchmarktest/benchmarktest.cpp)

sjson.hpp
---------

Single header json parser with some bonus features like comments and multiline
strings.

sweetql.hpp
---------

single header sqlite3 wrapper that makes the conversion from sql table to
class and back again extremely simple with minimal typing and speed overhead.
Has an iterator interface that makes integration easy.

```C++
class ReservationPerson {
public:
	ReservationPerson() {}

	static SqlTable<ReservationPerson>& table() {
		static SqlTable<ReservationPerson> tab = 
			SqlTable<ReservationPerson>::sqlTable("ReservationPerson",
			SqlColumn<ReservationPerson>("Firstname", makeAttr(&ReservationPerson::firstname))
			SqlColumn<ReservationPerson>("age", makeAttr(&ReservationPerson::age))
		);

		return tab;
	};

	std::string firstname;
	long age;
};

...

	SqliteDB db("dbfilename");

	auto sel(db.select<ReservationPerson>());
	std::for_each(sel.first, sel.second, [&toDel](const ReservationPerson& p) {
	});
```

More examples in: [here](sweetqltest/sweetqltest.cpp)

options.hpp
---------

A single header cmd line option parser. With automatic type conversation, short
and long option names and pretty help printing.

Examples in: [here](optiontest/optiontest.cpp)


compare.hpp
-----------

With this header you can make type safe comparisions without taking care of
the types of the values compared.

```C++
bool eq = sweet::equal(1337u,1337)
bool neq = sweet::notEqual(1337u,1337)
```

Examples in: [here](comparetest/comparetest.cpp)

conv.hpp
---------

A D like "to" function thats safely converts single value types to another
value type.

```C++

int a = to<int>("some int value as string");
std::string a = to<std::string>(a);
```

More examples in: [here](convtest/convtest.cpp)

sweetcpp.py
---------

Tests if a given cpp file follows some random style rules

g2cpp.py
---------

Hand in a gtk glade file and get a class in a header file that builds that
widget and has protected data member pointing to all the widgets in the glade
file. On top of that a cast operator is implemented that casts the class to
the type of the top widget. 

trie.hpp
---------

A trie implementation that is ""not yet"" stl quality but does work.

Contributions
--------
Always welcome!

