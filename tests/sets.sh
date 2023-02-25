#!/bin/bash

. ./testing.libsh

# Immediate sets
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

# Semi-immediate sets (with non-immediate elements)
# testsemiset elemtype a_value b_value set lbound hbound
testsemiset() {
  echo "program foo; var i, a, b : $1;
        begin a := $2; b := $3; for i := $5 to $6 do if i in $4 then write(i, ' ') end."
}

# Test the different types of sets
testsemiset boolean false true "[]" false true | outputs ''
testsemiset boolean false true "[a]" false true | outputs 'FALSE '
testsemiset boolean false true "[b]" false true | outputs 'TRUE '
testsemiset boolean false true "[a..b]" false true | outputs 'FALSE TRUE '
testsemiset boolean false true "[b..a]" false true | outputs ''

testsemiset char "'e'" "'h'" "[]" "'a'" "'z'" | outputs ''
testsemiset char "'e'" "'h'" "['c', a..b]" "'a'" "'z'" | outputs 'c e f g h '
testsemiset char "'e'" "'h'" "[a..b, 'j']" "'a'" "'z'" | outputs 'e f g h j '
testsemiset char "'e'" "'h'" "['c', a, 'j']" "'a'" "'z'" | outputs 'c e j '
testsemiset char "'e'" "'h'" "['c', a..b, 'j']" "'a'" "'z'" | outputs 'c e f g h j '

testsemiset integer 5 7 "[]" 1 10 | outputs ''
testsemiset integer 5 7 "[1,a..b]" 1 10 | outputs '1 5 6 7 '
testsemiset integer 5 7 "[a..b, 10]" 1 10 | outputs '5 6 7 10 '
testsemiset integer 5 7 "[1, a..b, 10]" 1 10 | outputs '1 5 6 7 10 '
testsemiset integer 5 7 "[1, a, 10]" 1 10 | outputs '1 5 10 '

# Operations on sets
# testsemisetop elemtype a_value b_value l_set r_set op lbound hbound
testsemisetop() {
  echo "program foo; var i, a, b : $1; l, r : set of $7..$8;
        begin a := $2; b := $3; l := $4; r := $5;
        for i := $7 to $8 do if i in (l $6 r) then write(i, ' ') end."
}
testsemisetop integer 3 4 "[]" "[]" '+' 1 10 | outputs ''
testsemisetop integer 3 4 "[1..a]" "[]" '+' 1 10 | outputs '1 2 3 '
testsemisetop integer 3 4 "[]" "[b..b+2]" '+' 1 10 | outputs '4 5 6 '
testsemisetop integer 3 4 "[1..a]" "[4..6]" '+' 1 10 | outputs '1 2 3 4 5 6 '
testsemisetop integer 3 4 "[1..b+1]" "[a..6]" '+' 1 10 | outputs '1 2 3 4 5 6 '
testsemisetop integer 3 4 "[1]" "[a]" '+' 1 10 | outputs '1 3 '

testsemisetop integer 3 4 "[]" "[]" '-' 1 10 | outputs ''
testsemisetop integer 3 4 "[1..a]" "[]" '-' 1 10 | outputs '1 2 3 '
testsemisetop integer 3 4 "[]" "[b..b+2]" '-' 1 10 | outputs ''
testsemisetop integer 3 4 "[1..a]" "[4..6]" '-' 1 10 | outputs '1 2 3 '
testsemisetop integer 3 4 "[1..b+1]" "[a..6]" '-' 1 10 | outputs '1 2 '
testsemisetop integer 3 4 "[1]" "[a]" '-' 1 10 | outputs '1 '

testsemisetop integer 3 4 "[]" "[]" '*' 1 10 | outputs ''
testsemisetop integer 3 4 "[1..a]" "[]" '*' 1 10 | outputs ''
testsemisetop integer 3 4 "[]" "[b..b+2]" '*' 1 10 | outputs ''
testsemisetop integer 3 4 "[1..a]" "[4..6]" '*' 1 10 | outputs ''
testsemisetop integer 3 4 "[1..b+1]" "[a..6]" '*' 1 10 | outputs '3 4 5 '
testsemisetop integer 3 4 "[1]" "[a]" '*' 1 10 | outputs ''

# Sets returned by expressions
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

# testvarset elemtype set lbound hbound
testvarset() {
  echo "program foo; var i : $1; s : set of $1;
        begin s := $2; for i := $3 to $4 do if i in s then write(i, ' ') end."
}

# Test the different types of sets
testvarset boolean "[]" false true | outputs ''
testvarset boolean "[false]" false true | outputs 'FALSE '
testvarset boolean "[true]" false true | outputs 'TRUE '
testvarset boolean "[false..true]" false true | outputs 'FALSE TRUE '

testvarset char "[]" "'a'" "'z'" | outputs ''
testvarset char "['a','b','e']" "'a'" "'z'" | outputs 'a b e '
testvarset char "['a'..'e']" "'a'" "'z'" | outputs 'a b c d e '
testvarset char "['a'..'c','e']" "'a'" "'z'" | outputs 'a b c e '
testvarset char "['a', 'c'..'e']" "'a'" "'z'" | outputs 'a c d e '

testvarset 1..10 "[]" 1 10 | outputs ''
testvarset 1..10 "[1,3,5]" 1 10 | outputs '1 3 5 '
testvarset 1..10 "[1..3,5]" 1 10 | outputs '1 2 3 5 '
testvarset 1..10 "[1,3..5]" 1 10 | outputs '1 3 4 5 '

testvarset string "['abc']" "'a'" "'z'" | is_not_valid
testvarset 1..10 "[1.4]" 1 10 | is_not_valid

# Operations on sets
testvarset 100..200 "[] + []" 100 200 | outputs ''
testvarset 100..200 "[101..103] + []" 100 200 | outputs '101 102 103 '
testvarset 100..200 "[] + [194..196]" 100 200 | outputs '194 195 196 '
testvarset 100..200 "[101..103] + [194..196]" 100 200 | outputs '101 102 103 194 195 196 '

testvarset 100..200 "[] - []" 100 200 | outputs ''
testvarset 100..200 "[101..103] - []" 100 200 | outputs '101 102 103 '
testvarset 100..200 "[] - [194..196]" 100 200 | outputs ''
testvarset 100..200 "[101..103] - [194..196]" 100 200 | outputs '101 102 103 '
testvarset 100..200 "[101..195] - [103..196]" 100 200 | outputs '101 102 '
testvarset 100..200 "[101] - [193]" 100 200 | outputs '101 '

testvarset 100..200 "[] * []" 100 200 | outputs ''
testvarset 100..200 "[101..103] * []" 100 200 | outputs ''
testvarset 100..200 "[] * [194..196]" 100 200 | outputs ''
testvarset 100..200 "[101..103] * [102..196]" 100 200 | outputs '102 103 '

# testvars a_type a_value b_type b_value expr
testvars() {
  echo "program foo; var a : $1; b : $3; begin a := $2; b := $4; write($5) end."
}

# Set comparisons
testvars "set of 0..10" "[]" "set of 0..10" "[]" "a = b" | outputs TRUE
testvars "set of 0..10" "[1..3]" "set of 0..10" "[1..3]" "a = b" | outputs TRUE
testvars "set of 0..10" "[1..4]" "set of 0..10" "[1..3]" "a = b" | outputs FALSE
testvars "set of 0..10" "[1..3]" "set of 0..10" "[1..4]" "a = b" | outputs FALSE
testvars "set of 0..10" "[2..3]" "set of 0..10" "[1..3]" "a = b" | outputs FALSE
testvars "set of 0..10" "[]" "set of 0..10" "[]" "a <> b" | outputs FALSE
testvars "set of 0..10" "[1..3]" "set of 0..10" "[1..3]" "a <> b" | outputs FALSE
testvars "set of 0..10" "[1..4]" "set of 0..10" "[1..3]" "a <> b" | outputs TRUE
testvars "set of 0..10" "[1..3]" "set of 0..10" "[1..4]" "a <> b" | outputs TRUE
testvars "set of 0..10" "[2..3]" "set of 0..10" "[1..3]" "a <> b" | outputs TRUE
testvars "set of 0..10" "[1..5]" "set of 0..10" "[1..5]" "a >= b" | outputs TRUE
testvars "set of 0..10" "[1..5]" "set of 0..10" "[2..4]" "a >= b" | outputs TRUE
testvars "set of 0..10" "[]" "set of 0..10" "[]" "a >= b" | outputs TRUE
testvars "set of 0..10" "[1..5]" "set of 0..10" "[]" "a >= b" | outputs TRUE
testvars "set of 0..10" "[]" "set of 0..10" "[1..5]" "a >= b" | outputs FALSE
testvars "set of 0..10" "[1..5]" "set of 0..10" "[1..6]" "a >= b" | outputs FALSE
testvars "set of 0..10" "[1..5]" "set of 0..10" "[0..5]" "a >= b" | outputs FALSE
testvars "set of 0..10" "[1..5]" "set of 0..10" "[1..5]" "a <= b" | outputs TRUE
testvars "set of 0..10" "[2..4]" "set of 0..10" "[1..5]" "a <= b" | outputs TRUE
testvars "set of 0..10" "[]" "set of 0..10" "[]" "a <= b" | outputs TRUE
testvars "set of 0..10" "[]" "set of 0..10" "[1..5]" "a <= b" | outputs TRUE
testvars "set of 0..10" "[1..5]" "set of 0..10" "[]" "a <= b" | outputs FALSE
testvars "set of 0..10" "[1..6]" "set of 0..10" "[1..1]" "a <= b" | outputs FALSE
testvars "set of 0..10" "[0..5]" "set of 0..10" "[1..5]" "a <= b" | outputs FALSE

# 'in' operator
testvars integer "1" "set of 0..10" "[]" "a in b" | outputs FALSE
testvars integer "1" "set of 0..10" "[1]" "a in b" | outputs TRUE
testvars integer "1" "set of 0..10" "[2]" "a in b" | outputs FALSE
testvars integer "1" "set of 0..10" "[0..2]" "a in b" | outputs TRUE
testvars integer "1" "set of 0..10" "[3..5]" "a in b" | outputs FALSE
