#!/usr/bin/python

import xml.etree.ElementTree as ET

names = []
nameCls = dict()

def recu(r):
	#print(r.tag, " ", stack)
	t = None
	if r.tag == "object":
		n = r.attrib["id"]
		nameCls[n] = r.attrib["class"]
		names.append(n)
		
	for c in r:
		recu(c)

def buildConstructMethods(f):
	f.write("\nprivate:\n")
	for n in names[1:]:
		if n[0:3] == "tab":
			continue
		if n[0:len("placeholder")] == "placeholder":
			continue

		if nameCls[n] == "GtkBox":
			continue

		f.write("\tinline void make_{}() {{\n".format(n))
		toCallFromConstruct.append("\t\tmake_{}();\n".format(n))
		for a in proper[n]:
			if a[0].find("pos") != -1:
				f.write("\t\t{}.set_{}({});\n".format(n, a[0], "POS_"+a[1].upper()))
			elif a[0].find("orientation") != -1:
				f.write("\t\t{}.set_{}({});\n".format(n, a[0], "ORIENTATION_"+a[1].upper()))
			elif(a[1].lower() == "true" or a[1].lower() == "false" 
					or isConvToDouble(a[1])):
				f.write("\t\t{}.set_{}({});\n".format(n, a[0], a[1].lower()))
			else:
				f.write("\t\t{}.set_{}({});\n".format(n, a[0], '"'+a[1].lower()+'"'))

		f.write("\t\t{}.show();\n".format(n))

		f.write("\t}\n\n")

def buildDataMember(f):
	f.write("protected:\n")
	f.write("\tGlib::RefPtr<Gtk::Builder> _builder;\n")
	for n in names:
		if n[0:3] == "tab":
			continue
		f.write("\tGtk::{}* {};\n".format(nameCls[n][3:], n))

def buildConstructorAndCast(f, cn):
	f.write("\npublic:\n")
	f.write("\tinline {}() {{\n".format(cn))
	f.write("\t\t_builder = Gtk::Builder::create_from_string(glade_file);\n")
	for i in names:
		f.write("\t\t_builder->get_widget(\"{}\", {});\n".format(i, i))
	f.write("\t}\n\n")

	f.write("\tinline operator {}*() {{\n".format(nameCls[names[0]][3:]))
	f.write("\t\treturn {};\n".format(names[0]))
	f.write("\t}\n")
	

def printer(fn,cn,pc):
	f = open(fn, "w")
	text = open(cn+".glade", "r")
	f.write("#ifndef {}_HPP\n"
		"#define {}_HPP\n\n"
		"#include <gtkmm.h>\n\n"
		"using namespace Gtk;\n\n"
		"class {} {{\n".format(cn, cn, pc))
	f.write("\tconst char* glade_file = \n")
	for l in text:
		f.write('"'+l.replace('"', '\\"')[:-1] + "\"\n")
	f.write(";\n")
	buildDataMember(f)
	buildConstructorAndCast(f, cn)
	f.write("};\n\n#endif\n")


fn = "glade"

tree = ET.parse(fn+".glade")
root = tree.getroot()[0]
recu(root)

print(names)
print("\nproperites")

printer(fn+".hpp",fn,fn)
