#!/bin/bash

. ./testing.libsh

testexpr() {
  echo "program foo; begin write($1) end."
}

testexpr 'abs(3)' | outputs '3'
testexpr 'abs(-3)' | outputs '3'
testexpr 'abs(3.1)' | outputs '3.1'
testexpr 'abs(-3.1)' | outputs '3.1'
testexpr 'sqr(3)' | outputs '9'
testexpr 'sqr(3.1)' | outputs '9.61'
testexpr 'sin(0)' | outputs '0'
testexpr 'sin(1)' | outputs '0.841471'
testexpr 'sin(2)' | outputs '0.909297'
testexpr 'cos(0)' | outputs '1'
testexpr 'cos(1)' | outputs '0.540302'
testexpr 'cos(2)' | outputs '-0.416147'
testexpr 'exp(0)' | outputs '1'
testexpr 'exp(1)' | outputs '2.71828'
testexpr 'exp(-1)' | outputs '0.367879'
testexpr 'ln(1)' | outputs '0'
testexpr 'ln(2)' | outputs '0.693147'
testexpr 'sqrt(1)' | outputs '1'
testexpr 'sqrt(2)' | outputs '1.41421'
testexpr 'sqrt(4)' | outputs '2'
testexpr 'arctan(0)' | outputs '0'
testexpr 'arctan(1)' | outputs '0.785398'

testexpr 'frac(3.14)' | outputs '0.14'
testexpr 'frac(-3.14)' | outputs '-0.14'
testexpr 'int(3.14)' | outputs '3'
testexpr 'int(-3.14)' | outputs '-3'
testexpr 'round(3.4)' | outputs '3'
testexpr 'round(3.5)' | outputs '4'
testexpr 'round(-3.4)' | outputs '-3'
testexpr 'round(-3.5)' | outputs '-4'
testexpr 'trunc(3.14)' | outputs '3'
testexpr 'trunc(-3.14)' | outputs '-4'
