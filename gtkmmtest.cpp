#include <gtkmm.h>

#include "glade.hpp"

//  g++ gtkmmtest.cpp -o gtkmmtest `pkg-config gtkmm-3.0 --cflags --libs` --std=c++11

int main(int argc, char** argv) {
	Gtk::Main kit(argc, argv);
	glade g;
	Gtk::Window* w = g;
	Gtk::Main::run(*w);
	return 0;
}
