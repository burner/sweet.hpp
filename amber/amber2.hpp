#pragma once

#include <ostream>



template<typename O, typename P>
void amber2(O& out, P& params) {
	out<<"<!DOCTYPE html>\n";
	out<<"\n<html>";
	out<<"\n\t<head>";
	out<<"\n\t\t<title>";
	out<<"\n\t\t\tA web page ";

	out<<"\n\t\t</title>";
	out<<"\n\t</head>";
	out<<"\n\t<body>";
	out<<"\n\t\t<h1 class=\"foo\">";
	out<<"\n\t\t\tA web page made with amber ";

	out<<"\n\t\t</h1>";
	out<<"\n\t\t<h2>";
	out<<"\n\t\t\tAmber can create webpages very fast ";

	out<<"\n\t\t</h2>";
	out<<"\n\t\t<ul>";
	out<<"\n\t\t\t<li>";
	out<<"\n\t\t\t\tNative C++ HTML Generator ";

	out<<"\n\t\t\t</li>";
	out<<"\n\t\t\t<li>";
	out<<"\n\t\t\t\tCompletly Programmable ";

	out<<"\n\t\t\t</li>";
	out<<"\n\t\t\t<li>";
	out<<"\n\t\t\t\tEasy to begin ";

	out<<"\n\t\t\t</li>";
	out<<"\n\t\t</ul>";
	out<<"\n\t\t<ul>";
	for(size_t i = 0; i < 5; ++i) {
	out<<"\n\t\t\t<li>";
	out<<"\n\t\t\t\tItem ";
	out<<"\t\t\t\t"<<( i );

	out<<"\n\t\t\t</li>";
	}
	out<<"\n\t\t</ul>";
	out<<"\n\t</body>";
	out<<"\n</html>";
}
