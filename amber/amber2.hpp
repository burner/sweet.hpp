#pragma once

#include <ostream>

//#include "someHeader.hpp"

template<typename O, typename P>
void amber2(O& out, P& params) {
	out<<"<foo>\n";
	out<<"<bar class=\"args\">\n";
	out<<"some text about stuff";

	out<<"</bar>\n";
	out<<"<bzz id=\"hello\" class=\"args\">\n";
	out<<"</bzz>\n";
	out<<"<brr id=\"hello\">\n";
	out<<"</brr>\n";
	out<<"<wdv hello=\"asdj\" bar=\"fjasd\">\n";
	out<<"</wdv>\n";
	out<<"<baz>\n";
	out<<"this is some longer string that may";

	out<<"span multiple lines";

	out<<"<zzzz>\n";
	out<<"</zzzz>\n";
	out<<"<yyyy/>";
	out<<"</baz>\n";
	out<<"<openclose/>";
	out<<"<openclose id=\"bar\" class=\"foo\"/>";
	out<<"<ul>\n";
	out<<"<li>\n";
	out<<"Item A ";

	out<<"</li>\n";
	out<<"<li>\n";
	out<<"Item B ";

	out<<"</li>\n";
	out<<"<li>\n";
	out<<"Item C ";

	out<<"</li>\n";
	out<<"</ul>\n";
	out<<"<h1>\n";
	out<<( params );

	out<<"</h1>\n";
	out<<"<p>\n";
	out<<"Some text describing something";

	out<<"<ul>\n";
	for(size_t i = 0; i < 2; ++i) { // a single c++ line
	out<<"<li>\n";
	out<<"Item ";
	out<<( i );

	out<<"</li>\n";
	out<<"<li>\n";
	out<<( i );
	out<<"Item ";

	out<<"</li>\n";
	out<<"<li>\n";
	out<<"Item ";
	out<<( i );
	out<<"Item ";

	out<<"</li>\n";
	}
	out<<"</ul>\n";
	out<<"</p>\n";
	out<<"</foo>\n";
}
