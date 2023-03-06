#!/bin/bash

. ./testing.libsh

testexpr() {
  echo "program foo; begin write($1) end."
}

testexpr 'abs(3)' | outputs '3'
testexpr 'abs(-3)' | outputs '3'
testexpr 'abs(3.1)' | outputs ' 3.10000000000000E+000'
testexpr 'abs(-3.1)' | outputs ' 3.10000000000000E+000'
testexpr 'sqr(3)' | outputs '9'
testexpr 'sqr(3.1)' | outputs ' 9.61000000000000E+000'
testexpr 'sin(0)' | outputs ' 0.00000000000000E+000'
testexpr 'sin(1)' | outputs ' 8.41470984807897E-001'
testexpr 'sin(2)' | outputs ' 9.09297426825682E-001'
testexpr 'cos(0)' | outputs ' 1.00000000000000E+000'
testexpr 'cos(1)' | outputs ' 5.40302305868140E-001'
testexpr 'cos(2)' | outputs '-4.16146836547142E-001'
testexpr 'exp(0)' | outputs ' 1.00000000000000E+000'
testexpr 'exp(1)' | outputs ' 2.71828182845905E+000'
testexpr 'exp(-1)' | outputs ' 3.67879441171442E-001'
testexpr 'ln(1)' | outputs ' 0.00000000000000E+000'
testexpr 'ln(2)' | outputs ' 6.93147180559945E-001'
testexpr 'sqrt(1)' | outputs ' 1.00000000000000E+000'
testexpr 'sqrt(2)' | outputs ' 1.41421356237310E+000'
testexpr 'sqrt(4)' | outputs ' 2.00000000000000E+000'
testexpr 'arctan(0)' | outputs ' 0.00000000000000E+000'
testexpr 'arctan(1)' | outputs ' 7.85398163397448E-001'

testexpr 'frac(3.14)' | outputs ' 1.40000000000000E-001'
testexpr 'frac(-3.14)' | outputs '-1.40000000000000E-001'
testexpr 'int(3.14)' | outputs ' 3.00000000000000E+000'
testexpr 'int(-3.14)' | outputs '-3.00000000000000E+000'
testexpr 'round(3.4)' | outputs '3'
testexpr 'round(3.5)' | outputs '4'
testexpr 'round(-3.4)' | outputs '-3'
testexpr 'round(-3.5)' | outputs '-4'
testexpr 'trunc(3.14)' | outputs '3'
testexpr 'trunc(-3.14)' | outputs '-4'
