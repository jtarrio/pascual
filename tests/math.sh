#!/bin/bash

. ./testing.libsh

testexpr() {
  echo "program foo; begin write($1) end."
}

testexpr 'abs(3)' | outputs '3'
testexpr 'abs(-3)' | outputs '3'
testexpr 'abs(3.1)' | outputs '  3.1000000000E+00'
testexpr 'abs(-3.1)' | outputs '  3.1000000000E+00'
testexpr 'sqr(3)' | outputs '9'
testexpr 'sqr(3.1)' | outputs '  9.6100000000E+00'
testexpr 'sin(0)' | outputs '  0.0000000000E+00'
testexpr 'sin(1)' | outputs '  8.4147098481E-01'
testexpr 'sin(2)' | outputs '  9.0929742683E-01'
testexpr 'cos(0)' | outputs '  1.0000000000E+00'
testexpr 'cos(1)' | outputs '  5.4030230587E-01'
testexpr 'cos(2)' | outputs ' -4.1614683655E-01'
testexpr 'exp(0)' | outputs '  1.0000000000E+00'
testexpr 'exp(1)' | outputs '  2.7182818285E+00'
testexpr 'exp(-1)' | outputs '  3.6787944117E-01'
testexpr 'ln(1)' | outputs '  0.0000000000E+00'
testexpr 'ln(2)' | outputs '  6.9314718056E-01'
testexpr 'sqrt(1)' | outputs '  1.0000000000E+00'
testexpr 'sqrt(2)' | outputs '  1.4142135624E+00'
testexpr 'sqrt(4)' | outputs '  2.0000000000E+00'
testexpr 'arctan(0)' | outputs '  0.0000000000E+00'
testexpr 'arctan(1)' | outputs '  7.8539816340E-01'

testexpr 'frac(3.14)' | outputs '  1.4000000000E-01'
testexpr 'frac(-3.14)' | outputs ' -1.4000000000E-01'
testexpr 'int(3.14)' | outputs '  3.0000000000E+00'
testexpr 'int(-3.14)' | outputs ' -3.0000000000E+00'
testexpr 'round(3.4)' | outputs '3'
testexpr 'round(3.5)' | outputs '4'
testexpr 'round(-3.4)' | outputs '-3'
testexpr 'round(-3.5)' | outputs '-4'
testexpr 'trunc(3.14)' | outputs '3'
testexpr 'trunc(-3.14)' | outputs '-4'
