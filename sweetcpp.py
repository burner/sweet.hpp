#!/usr/bin/python
import argparse
import re
import math

class TabIndentTest:
	def __init__(self):
		self.reg = re.compile("^ ")

	def test(self, line):
		return self.reg.search(line) == None
		
	def msg(self):
		return "Line is indented with blanks, change to tabulator."

class WhiteSpaceAtEnd:
	def __init__(self):
		self.reg = re.compile("[ \t]+$")

	def test(self, line):
		return self.reg.search(line) == None
		
	def msg(self):
		return "Line has whitespace at the end, remove it."

class BraceBeginsLine:
	def __init__(self):
		self.reg = re.compile("^[ \t]+{")

	def test(self, line):
		return self.reg.search(line) == None
		
	def msg(self):
		return "A { should not start a line"

class IfParent:
	def __init__(self):
		self.reg = re.compile("[ \t]+if[ \t]+")

	def test(self, line):
		return self.reg.search(line) == None
		
	def msg(self):
		return "If not properly formated. Format is if(...)."

class ElseBrace:
	def __init__(self):
		self.reg1 = re.compile("[ \t]+else[ \t]+")
		self.reg2 = re.compile("^[\t]+} else {$")

	def test(self, line):
		if self.reg1.search(line) != None:
			return self.reg2.search(line) != None
		else:
			return True
		
	def msg(self):
		return "Else not formated properly. Should be '} else {'."

class LineLen:
	def __init__(self):
		pass

	def test(self, line):
		l = len(line)
		for i in line:
			if i == '\t':
				l+=3

		if l < 79:
			return True
		else:
			return False
		
	def msg(self):
		return "Line is longer than 78. Format code differently."

def parseArgs():
	parser = argparse.ArgumentParser(description="Sweet Cpp Style Checker")
	parser.add_argument("filesToTest", metavar="N", type=str, nargs="*",
		help="Files that should be checked")
	
	args = parser.parse_args();
	return args

def shortenFilename(name):
	s = name.rfind('/')
	if s == -1:
		return name
	else:
		return name[s:]

tests = [TabIndentTest(), WhiteSpaceAtEnd(), BraceBeginsLine(), IfParent(),
ElseBrace(), LineLen()]

if __name__ == "__main__":
	args = parseArgs()

	lenLongestTestName = len(max(tests, key=lambda x: x.__class__.__name__)
		.__class__.__name__)

	for File in args.filesToTest:
		sFn = shortenFilename(File)
		lines = open(File).readlines()
		numOfLines = int(math.log10(len(lines))+1)
		lineNumber = 0
		for line in lines:
			for test in tests:
				if not test.test(line):
					print("{tn:{tl}} at {n}:{pos:{ln}} | {msg}".format(
						tn = test.__class__.__name__, pos=lineNumber, 
						msg = test.msg(), tl=lenLongestTestName,
						ln = numOfLines,n=sFn)
					)

			lineNumber+=1
