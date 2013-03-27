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

def parseArgs():
	parser = argparse.ArgumentParser(description="Sweet Cpp Style Checker")
	parser.add_argument("filesToTest", metavar="N", type=str, nargs="*",
		help="Files that should be checked")
	
	args = parser.parse_args();
	return args

tests = [TabIndentTest(), WhiteSpaceAtEnd(), BraceBeginsLine()]

if __name__ == "__main__":
	args = parseArgs()

	lenLongestTestName = len(max(tests, key=lambda x: x.__class__.__name__)
		.__class__.__name__)

	for File in args.filesToTest:
		lines = open(File).readlines()
		numOfLines = int(math.log10(len(lines))+1)
		lineNumber = 0
		for line in lines:
			for test in tests:
				if not test.test(line):
					print("{tn:{tl}} at {pos:{ln}} : {msg}".format(
						tn = test.__class__.__name__, pos=lineNumber, 
						msg = test.msg(), tl=lenLongestTestName,
						ln = numOfLines)
					)

			lineNumber+=1
