#!/bin/bash

. ./testing.libsh

testtype() {
  echo "program foo; type mytype = $1; begin write(sizeof(mytype)) end."
}
# Built-in types
testtype boolean | outputs 4
testtype integer | outputs 4
testtype real | outputs 8
testtype char | outputs 1
testtype string | outputs 256

# Ranges
testtype 0..255 | outputs 1
testtype -128..127 | outputs 1
testtype 0..65535 | outputs 2
testtype -32768..32767 | outputs 2
testtype -2147483648..2147483647 | outputs 4
testtype 0..2147483647 | outputs 4

# Enums
testtype '(One, Two, Three)' | outputs 1

# Arrays
testtype 'array[1..10] of -128..127' | outputs 10
testtype 'packed array[1..10] of -128..127' | outputs 10
testtype 'array[1..10,1..100] of 0..65535' | outputs 2000
testtype 'packed array[1..10,1..100] of 0..65535' | outputs 2000

# Records
testtype 'record a : -128..127; b : 0..65535; c : integer; end' | outputs 8
testtype 'packed record a : -128..127; b : 0..65535; c : integer; end' | outputs 7
