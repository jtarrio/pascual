#!/bin/bash

. ./testing.libsh

# testimmset elemtype set lbound hbound
testimmset() {
  echo "program foo; var i : $1;
        begin for i := $3 to $4 do if i in $2 then write(i, ' ') end."
}

# Test the different types of sets
testimmset boolean "[]" false true | outputs ''
testimmset boolean "[false]" false true | outputs 'FALSE '
testimmset boolean "[true]" false true | outputs 'TRUE '
testimmset boolean "[false..true]" false true | outputs 'FALSE TRUE '

testimmset char "[]" "'a'" "'z'" | outputs ''
testimmset char "['a','b','e']" "'a'" "'z'" | outputs 'a b e '
testimmset char "['a'..'e']" "'a'" "'z'" | outputs 'a b c d e '
testimmset char "['a'..'c','e']" "'a'" "'z'" | outputs 'a b c e '
testimmset char "['a', 'c'..'e']" "'a'" "'z'" | outputs 'a c d e '

testimmset integer "[]" 1 10 | outputs ''
testimmset integer "[1,3,5]" 1 10 | outputs '1 3 5 '
testimmset integer "[1..3,5]" 1 10 | outputs '1 2 3 5 '
testimmset integer "[1,3..5]" 1 10 | outputs '1 3 4 5 '

testimmset string "['abc']" "'a'" "'z'" | is_not_valid
testimmset integer "[1.4]" 1 10 | is_not_valid

# Operations on sets
testimmset integer "[] + []" 1 10 | outputs ''
testimmset integer "[1..3] + []" 1 10 | outputs '1 2 3 '
testimmset integer "[] + [4..6]" 1 10 | outputs '4 5 6 '
testimmset integer "[1..3] + [4..6]" 1 10 | outputs '1 2 3 4 5 6 '
testimmset integer "[1..5] + [3..6]" 1 10 | outputs '1 2 3 4 5 6 '
testimmset integer "[1] + [3]" 1 10 | outputs '1 3 '

testimmset integer "[] - []" 1 10 | outputs ''
testimmset integer "[1..3] - []" 1 10 | outputs '1 2 3 '
testimmset integer "[] - [4..6]" 1 10 | outputs ''
testimmset integer "[1..3] - [4..6]" 1 10 | outputs '1 2 3 '
testimmset integer "[1..5] - [3..6]" 1 10 | outputs '1 2 '
testimmset integer "[1] - [3]" 1 10 | outputs '1 '

testimmset integer "[] * []" 1 10 | outputs ''
testimmset integer "[1..3] * []" 1 10 | outputs ''
testimmset integer "[] * [4..6]" 1 10 | outputs ''
testimmset integer "[1..3] * [4..6]" 1 10 | outputs ''
testimmset integer "[1..5] * [3..6]" 1 10 | outputs '3 4 5 '
testimmset integer "[1] * [3]" 1 10 | outputs ''

# testexpr expr
testexpr() {
  echo "program foo; begin write($1) end."
}

# Set comparisons
testexpr "[] = []" | outputs TRUE
testexpr "[1..3] = [1..3]" | outputs TRUE
testexpr "[1..4] = [1..3]" | outputs FALSE
testexpr "[1..3] = [1..4]" | outputs FALSE
testexpr "[2..3] = [1..3]" | outputs FALSE
testexpr "[] <> []" | outputs FALSE
testexpr "[1..3] <> [1..3]" | outputs FALSE
testexpr "[1..4] <> [1..3]" | outputs TRUE
testexpr "[1..3] <> [1..4]" | outputs TRUE
testexpr "[2..3] <> [1..3]" | outputs TRUE
testexpr "[1..5] >= [1..5]" | outputs TRUE
testexpr "[1..5] >= [2..4]" | outputs TRUE
testexpr "[] >= []" | outputs TRUE
testexpr "[1..5] >= []" | outputs TRUE
testexpr "[] >= [1..5]" | outputs FALSE
testexpr "[1..5] >= [1..6]" | outputs FALSE
testexpr "[1..5] >= [0..5]" | outputs FALSE
testexpr "[1..5] <= [1..5]" | outputs TRUE
testexpr "[2..4] <= [1..5]" | outputs TRUE
testexpr "[] <= []" | outputs TRUE
testexpr "[] <= [1..5]" | outputs TRUE
testexpr "[1..5] <= []" | outputs FALSE
testexpr "[1..6] <= [1..1]" | outputs FALSE
testexpr "[0..5] <= [1..5]" | outputs FALSE

# 'in' operator
testexpr "1 in []" | outputs FALSE
testexpr "1 in [1]" | outputs TRUE
testexpr "1 in [2]" | outputs FALSE
testexpr "1 in [0..2]" | outputs TRUE
testexpr "1 in [3..5]" | outputs FALSE
