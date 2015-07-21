#include <iostream>
#include <death.hpp>
#include <iostream>

struct Foo {
	int a = 0;
};

Foo* get() {
	Foo a;
	return &a;
}
