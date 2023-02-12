#!/bin/bash

. ./testing.libsh

# testdef arraydef
# testdef typedef arraydef
testdef() {
  if [ -z "$2" ]
  then
    echo "program foo; var Arr : $1; begin end."
  else
    echo "program foo; type T = $1; var Arr : $2; begin end."
  fi
}

testdef 'array[1] of integer' | is_not_valid
testdef 'array[1..10] of integer' | is_valid
testdef 'array[1..1] of integer' | is_valid
testdef 'array[10..1] of integer' | is_not_valid
testdef 'array[5..15] of integer' | is_valid
testdef 'array[-5..5] of integer' | is_valid

testdef 'array[1..5] of array[1..10] of integer' | is_valid
testdef 'array[1..5, 1..10] of integer' | is_valid

testdef 'array[Boolean] of integer' | is_valid
testdef 'array[Char] of integer' | is_valid

testdef '(A, B, C, D, E, F)' 'array[B..E] of integer' | is_valid

# testelem arraydef element_index value
# testelem typedef arraydef element_index value
testelem() {
  if [ -z "$4" ]
  then
    echo "program foo; var Arr : $1; begin Arr[$2] := $3; write(Arr[$2]) end."
  else
    echo "program foo; type T = $1; var Arr : $2;
          begin Arr[$3] := $4; write(Arr[$3]) end."
  fi
}

testelem 'array[1..3] of integer' 0 123 | is_not_valid
testelem 'array[1..3] of integer' 1 123 | outputs 123
testelem 'array[1..3] of integer' 2 234 | outputs 234
testelem 'array[1..3] of integer' 3 345 | outputs 345
testelem 'array[1..3] of integer' 4 123 | is_not_valid
testelem 'array[5..15] of integer' 4 123 | is_not_valid
testelem 'array[5..15] of integer' 5 123 | outputs 123
testelem 'array[5..15] of integer' 15 123 | outputs 123
testelem 'array[5..15] of integer' 16 123 | is_not_valid
testelem 'array[-5..5] of integer' -6 123 | is_not_valid
testelem 'array[-5..5] of integer' -5 123 | outputs 123
testelem 'array[-5..5] of integer' 5 123 | outputs 123
testelem 'array[-5..5] of integer' 6 123 | is_not_valid
testelem 'array[1..10] of array[20..30] of integer' '1][20' 123 | outputs 123
testelem 'array[1..10] of array[20..30] of integer' '1][30' 123 | outputs 123
testelem 'array[1..10] of array[20..30] of integer' '1][31' 123 | is_not_valid
testelem 'array[1..10] of array[20..30] of integer' '10][20' 123 | outputs 123
testelem 'array[1..10] of array[20..30] of integer' '11][20' 123 | is_not_valid
testelem 'array[1..10] of array[20..30] of integer' '1, 20' 123 | will_be_valid
testelem 'array[1..10] of array[20..30] of integer' '1, 30' 123 | will_be_valid
testelem 'array[1..10] of array[20..30] of integer' '1, 31' 123 | will_be_valid
testelem 'array[1..10] of array[20..30] of integer' '10, 20' 123 | will_be_valid
testelem 'array[1..10] of array[20..30] of integer' '11, 20' 123 | will_be_valid
testelem 'array[1..10, 20..30] of integer' '1][20' 123 | outputs 123
testelem 'array[1..10, 20..30] of integer' '1][30' 123 | outputs 123
testelem 'array[1..10, 20..30] of integer' '1][31' 123 | is_not_valid
testelem 'array[1..10, 20..30] of integer' '10][20' 123 | outputs 123
testelem 'array[1..10, 20..30] of integer' '11][20' 123 | is_not_valid
testelem 'array[1..10, 20..30] of integer' '1, 20' 123 | will_be_valid
testelem 'array[1..10, 20..30] of integer' '1, 30' 123 | will_be_valid
testelem 'array[1..10, 20..30] of integer' '1, 31' 123 | will_be_valid
testelem 'array[1..10, 20..30] of integer' '10, 20' 123 | will_be_valid
testelem 'array[1..10, 20..30] of integer' '11, 20' 123 | will_be_valid

testelem 'array[1..3] of char' 1 "'a'" | outputs a
testelem 'array[1..3] of string' 1 "'abcde'" | outputs abcde

testelem '(A, B, C, D, E)' 'array[B..D] of integer' B 1 | outputs 1
testelem '(A, B, C, D, E)' 'array[B..D] of integer' C 2 | outputs 2
testelem '(A, B, C, D, E)' 'array[B..D] of integer' D 3 | outputs 3

echo 'program foo; var A : array[1..5] of integer; I : 2..4;
      begin I := 3; A[I] := 5; write(A[I]) end.' | outputs 5
