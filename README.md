SWEET_HPP
=========

Usage
-----
To use the header, just make the sweet.hpp folder an include for your compiler.

Example
------
Usually there is a folder called HEADERNAMEtest that holds an example. 

Contributions
--------
Always welcome!

Header
--------

Sweet cpp header that help in daily life

unit.hpp
--------
 extremely small unit testing framework 

```C++
UNITTEST(InformativTestName) {
	// your test here
}

int main() {
	if(!Unit::runTests()) {
		std::cerr<<"Some errors occoured"<<std::endl;
	}
	return 0;
}
```

 format.hpp
------------

 variadic template printf string formatting 

logger.hpp 
------------ 

easy to use logger
```C++
void someRandomFunction() {
	LOG();
	LOG("Hello");
	LOG("Hello %s", "World");
}
```

dbc.hpp
---------

We have unittest maybe some Design by Contract might also.

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
For a more in depth example take a look into the dbctest folder.


benchmark.hpp
---------

Benchmark structs that limits typing work, a Bench struct that stops time and
a macro that stops time in a Compound Statement in a thread save manner.
```
#include <benchmark.hpp>

void fun() {
	BENCH(fun);
	...
}

int main() {
	Bench t; // Simple time stopping struct

	Benchmark::printResults(); // print results of BENCH marcos

	std::cout<<t.milli();
}
```

sjson.hpp
---------

single header json parser

sweetql.hpp
---------

single header sqlite3 wrapper that makes the conversion from sql table to
class and back again extremely simple with minimal typing and speed overhead.
Has an iterator interface that makes integration easy.

options.hpp
---------

single header cmd line option parser. With automatic type conversation, short
and long option names and pretty help printing.

conv.hpp
---------

write `to<std::string>(variable)` and get a string. convenience wrapper around
stream operator

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
