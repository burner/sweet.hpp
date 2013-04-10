#!/usr/bin/python

import xml.etree.ElementTree as ET
import sys

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

def buildDataMember(f):
	f.write("protected:\n")
	f.write("\tGlib::RefPtr<Gtk::Builder> _builder;\n")
	for n in names:
		if n[0:3] == "tab":
			continue
		f.write("\tGtk::{}* {};\n".format(nameCls[n][3:], n))

def getCorrentFirstName():
	for i in names:
		if nameCls[i] == "GtkWindow" or nameCls[i] == "GtkDialog":
			return i	

def buildConstructorAndCast(f, cn):
	f.write("\npublic:\n")
	f.write("\tinline {}() {{\n".format(cn))
	f.write("\t\t_builder = Gtk::Builder::create_from_string(glade_file);\n")
	for i in names:
		f.write("\t\t_builder->get_widget(\"{}\", {});\n".format(i, i))
	f.write("\t}\n\n")

	#f.write("\tinline operator {}*() {{\n".format(nameCls[names[0]][3:]))
	#f.write("\t\treturn {};\n".format(names[0]))
	f.write("\tinline operator {}*() {{\n".format(nameCls[getCorrentFirstName()][3:]))
	f.write("\t\treturn {};\n".format(getCorrentFirstName()))
	f.write("\t}\n")
	

def printer(fn,cn,pc):
	h = fn.rfind("/")
	nfn = fn[:h+1]+"g2cpp_"+fn[h+1:]
	pci = pc.rfind("/")
	f = open(nfn, "w")
	text = open(cn, "r")
	f.write("#ifndef {}_HPP\n"
		"#define {}_HPP\n\n"
		"#include <gtkmm.h>\n\n"
		"using namespace Gtk;\n\n"
		"class {} {{\n".format("G2CPP_"+fn[h+1:-10].upper(), "G2CPP_"+fn[h+1:-10].upper(), pc[pci+1:-6]))
	f.write("\tconst char* glade_file = \n")
	for l in text:
		f.write('"'+l.replace('"', '\\"')[:-1] + "\"\n")
	f.write(";\n")
	buildDataMember(f)
	buildConstructorAndCast(f, pc[pci+1:-6])
	f.write("};\n\n#endif\n")


#fn = "glade"
fn = sys.argv[1]
print(fn)

tree = ET.parse(fn)
root = tree.getroot()
for i in root:
	recu(i)

#print(names)
#print("\nproperites")

printer(fn+".hpp",fn,fn)
