#!/usr/bin/python

from decimal import *
from random import *

def fun():
    ra = 100000000000000
    num = [ Decimal(str(randint(-ra,ra)) + "." + str(randint(0,ra))) for i in range(0,100)]

    print("UNITTEST(decimalTest1)) {")
    print("\tstd::vector<Decimal> nums = {");
    for i in num:
        print("\t\tDecimal(\"" + str(i) + "\"),")

    print("}\n")

if __name__ == "__main__":
    getcontext().prec = 13
    fun()
