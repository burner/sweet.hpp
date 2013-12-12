#!/usr/bin/python

from decimal import *
from random import *
import math

def fun():
	ra = [1000]
	#num = [ Decimal(str(randint(-a,a)) + "." + str(randint(0,a))) for a in ra for i in range(0,25)]
	num = [ uniform(0,a) for a in ra for i in range(0,25)]

	print("UNITTEST(decimalTest1) {")
	print("\tstd::vector<Fixed> nums = {");
	for i in num:
            print("\t\tFixed(\"{:.3f}\"),".format(i))

	print("\t}\n")
	for i in enumerate(num):
		for j in enumerate(num):
			print("\tAS_EQ(nums[{}] + nums[{}], Fixed({:.3f}));".format(i[0], j[0], i[1]+j[1]))
			print("\tAS_EQ(nums[{}] - nums[{}], Fixed({:.3f}));".format(i[0], j[0], i[1]-j[1]))
			if len(str(int(i[1]*j[1]))) < 17:
				print("\tAS_EQ(nums[{}] * nums[{}], Fixed({:.3f}));".format(i[0], j[0], i[1]*j[1]))
			print("\tAS_EQ(nums[{}] / nums[{}], Fixed({:.3f}));".format(i[0], j[0], i[1]/j[1]))
	print("}\n")

if __name__ == "__main__":
	getcontext().prec = 4
	fun()
