# Amber 

Amber is a [Jade-Lang](http://jade-lang.com/) inspired C++ HTML template
engine.
Amber takes a simple but powerful input lang and generates C++ that produces
HTML.

## Example

Talk is cheap, let us begin:

```
<{{
#include <boost/variant.hpp>
#include <unordered_map>
#include <string>
}}>

< html
	< head 
		< title A web page &{{ params["title"] }}&>
	>
	< body
		< h1.foo A web page made with Amber >

		Some line &{{ params["firstline"] }}&
		&{{ params["secondline"] }}& another line

		< ul 
		& for(int i = 0; i < params["runs"]; ++i) {
			<  li Item &{{ i }}& >
		& }
		>
	>
>
```
running
```
$ amber -i example.am -n example -o example.hpp
```
will create a file called "example.hpp" with the following content.
```
#pragma once

#include <ostream>

#include <boost/variant.hpp>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iostream>

template<typename O, typename P>
void example(O& out, P& params) {
	out<<"<!DOCTYPE html>\n";
	out<<"<html>\n";
	out<<"<head>\n";
	out<<"<title>\n";
	out<<"A web page ";
	out<<( params["title"] );

	out<<"</title>\n";
	out<<"</head>\n";
	out<<"<body>\n";
	out<<"<h1 class=\"foo\">\n";
	out<<"A web page made with Amber ";

	out<<"</h1>\n";
	out<<"Some line ";
	out<<( params["firstline"] );

	out<<( params["secondline"] );
	out<<"another line";

	out<<"<ul>\n";
	for(int i = 0; i < boost::get<int>(params["runs"]); ++i) {
	out<<"<li>\n";
	out<<"Item ";
	out<<( i );

	out<<"</li>\n";
	}
	out<<"</ul>\n";
	out<<"</body>\n";
	out<<"</html>\n";
}
```
This file contains a single function called *example*. The functions takes two
arguments. The first argument must be something that acts as an output stream.
The second argument can be anything, but the name params suggest it to be
parameters.

## Putting it all Together
Removing the first line adding a main function:
```
int main(int argc, char *argv[]) {
	std::unordered_map<std::string, boost::variant<std::string,int>> map;
	map["title"] = "Amber is awesome";
	map["firstline"] = "Some useful string";
	map["secondline"] = "Another line";
	map["runs"] = 2;

	std::stringstream out;
	example(out, map);

	std::cout<<out.str()<<std::endl;
	return 0;
}
```
and than compiling and running it should give you something like:

```
<!DOCTYPE html>
<html>
<head>
<title>
A web page Amber is awesome</title>
</head>
<body>
<h1 class="foo">
A web page made with Amber </h1>
Some line Some useful stringAnother lineanother line<ul>
<li>
Item 0</li>
<li>
Item 1</li>
</ul>
</body>
</html>
```
Dumping this into a file and opening it in a browser will give you a working
webpage.

## Grammar
Not complete yet!

```
Tag := < TagStart (Tag|CodeBlock|CodeLine|Text)* >

TagStart := Identifier
		| Identifier.classTag

		| Identifier.classTag#idTag

		| Identifier.classTag#idTag(node_attribute_list_ESCAPE_QUOTES)

		| Identifier#idTag

		| Identifier#idTag(node_attribute_list_ESCAPE_QUOTES)

		| Identifier(node_attribute_list_ESCAPE_QUOTES)

CodeBlock := <{{ .* }}>

CodeLine := \n&.*\n

Text := Some text plus InlineCPP more text\n

InlineCPP := &{{ some expression }}&
```
