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

int main() {
	sweet::DeathHandler dh;
	Foo* a = get();
	delete a;
	std::cout<<a->a<<std::endl;

	return 0;
}
