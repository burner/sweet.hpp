SWEET_HPP
=========

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

* benchmark.hpp
---------

benchmark struct that limits typing work

* sjson.hpp
---------

single header json parser

* sweetql.hpp
---------

single header sqlite3 wrapper that makes the conversion from sql table to class and back again extremely simple with minimal typing and speed overhead. Has an iterator interface that makes integration easy.

* options.hpp
---------

single header cmd line option parser. With automatic type conversation, short and long option names and pretty help printing.

* conv.hpp
---------

write `to<std::string>(variable)` and get a string. convenience wrapper around stream operator

* sweetcpp.py
---------

Tests if a given cpp file follows some random style rules

* g2cpp.py
---------

Hand in a gtk glade file and get a class in a header file that builds that widget and has protected data member pointing to all the widgets in the glade file. On top of that a cast operator is implemented that casts the class to the type of the top widget. 


Example
------
Usually there is a folder called HEADERNAMEtest that holds an example. 

Usage
-----
To use the header, just make the sweet.hpp folder an include for your compiler.

Contributions
--------
Always welcome!
