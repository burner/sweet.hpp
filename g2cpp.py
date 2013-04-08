#!/usr/bin/python

import xml.etree.ElementTree as ET

names = []
nameCls = dict()

def recu(r):
	#print(r.tag, " ", stack)
	t = None
	if r.tag == "object"
		n = r.attrib["id"]
		nameCls[n] = r.attrib["class"]
		names[n]

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
	for n in names[1:]:
		if n[0:3] == "tab":
			continue
		f.write("\tGtk::{} {};\n".format(nameCls[n][3:], n))

def buildBuildWidget(f):
	f.write("\nprivate:\n")
	f.write("\tinline void buildWidget() {\n")
	re = names[1:][::-1]
	for b in re:
		if b.startswith("tab"):
			continue
		elif b.startswith("placeholder"):
			continue
		for c in childs[b]:
			if c.startswith("placeholder"):
				continue
			elif nameCls[b].find("Window") != -1:
				f.write("\t\t{}.add({});\n".format(b, c))
			elif nameCls[b].find("Box") != -1:
				f.write("\t\t{}.pack_start({});\n".format(b, c))
			elif nameCls[b].find("Notebook") != -1:
				t = c[0:3]
				n = c[3:]
				f.write("\t\t{}.insert_page(placeholder{},label{},{});\n".format(b,n,n,n))
			elif nameCls[b].find("MenuItem"):
				f.write("\t\t{}.add({});\n".format(b,c))
			elif nameCls[b].find("MenuBar"):
				f.write("\t\t{}.append({});\n".format(b,c))
			elif nameCls[b].find("Menu"):
				f.write("\t\t{}.append({});\n".format(b,c))
				

	# main layout
	for c in childs[names[0]]:
		f.write("\t\tthis->add({});\n".format(c))

	f.write("\t}\n")

def buildConstructor(f,cn):
	f.write("public:\n")
	f.write("\tinline {}() {{\n".format(cn))
	for a in proper[names[0]]:
		if a[0].find("pos") != -1:
			f.write("\t\tthis->set_{}({});\n".format(a[0], "POS_"+a[1].upper()))
		elif(a[1].lower() == "true" or a[1].lower() == "false" 
				or isConvToDouble(a[1])):
			f.write("\t\tthis->set_{}({});\n".format(a[0], a[1].lower()))
		else:
			f.write("\t\tthis->set_{}({});\n".format(a[0], '"'+a[1].lower()+'"'))
		
	for i in toCallFromConstruct:
		f.write(i)

	f.write("\t\tbuildWidget();\n")
	f.write("\t\tthis->show_all();\n")
	f.write("\t}\n")

def printer(fn,cn,pc):
	f = open(fn, "w")
	f.write("#ifndef {}_HPP\n"
		"#define {}_HPP\n\n"
		"#include <gtkmm.h>\n\n"
		"using namespace Gtk;\n\n"
		"class {} : public Gtk::{} {{\n".format(cn, cn, cn, pc[3:]))
	buildDataMember(f)
	buildConstructMethods(f)
	buildConstructor(f, cn)
	buildBuildWidget(f)
	f.write("};\n\n#endif\n")


fn = "glade"

tree = ET.parse(fn+".glade")
root = tree.getroot()[0]
recu(root)

print(names)
print("\nproperites")
for i in proper:
	print(i," ",proper[i])

print("\nchilds")
for i in childs:
	print(i," ",childs[i])

printer(fn+".hpp",fn,nameCls[names[0]])
