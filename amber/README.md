# Amber 

Amber is a [Jade-Lang](http://jade-lang.com/) inspired C++ HTML template
engine.
Amber takes a simple but powerful input lang and generates C++ that produces
HTML.

## Example

Talk is cheap, let us begin.

```
<{{
#include "someheader.hpp"
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
