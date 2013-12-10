#!/usr/bin/python

from decimal import *
from random import *
import math

def fun():
	ra = [100000000000000,100000000,100000,1000]
	num = [ Decimal(str(randint(-a,a)) + "." + str(randint(0,a))) for a in ra for i in range(0,25)]

	print("UNITTEST(decimalTest1)) {")
	print("\tstd::vector<Decimal> nums = {");
	for i in num:
		print("\t\tDecimal(\"" + str(i) + "\"),")

	print("}\n")
	for i in enumerate(num):
		for j in enumerate(num):
			print("\tAS_EQ(nums[{}] + nums[{}], Decimal({:f}));".format(i[0], j[0], i[1]+j[1]))
			print("\tAS_EQ(nums[{}] - nums[{}], Decimal({:f}));".format(i[0], j[0], i[1]-j[1]))
			if len(str(int(i[1]*j[1]))) < 17:
				print("\tAS_EQ(nums[{}] * nums[{}], Decimal({:f}));".format(i[0], j[0], i[1]*j[1]))
			print("\tAS_EQ(nums[{}] / nums[{}], Decimal({:f}));".format(i[0], j[0], i[1]/j[1]))
	print("}\n")

if __name__ == "__main__":
	getcontext().prec = 13
	fun()
