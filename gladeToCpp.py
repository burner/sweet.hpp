#!/usr/bin/python

import xml.etree.ElementTree as ET

def recu(r):
	print(r.tag)
	print(r.attrib)
	print(r.text)
	print()
	for c in r:
		recu(c)

tree = ET.parse("glade.glade")
root = tree.getroot()
recu(root)
