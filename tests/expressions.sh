#!/bin/bash

. ./testing.libsh

# runexpr expr type
# runexpr defs init expr type
runexpr() {
  echo 'program foo;'
  if [ -n "$3" ]
  then
    echo "$1; var _res : $4; begin $2; _res := $3; write(_res) end."
  else
    echo "var _res : $2; begin _res := $1; write(_res) end."
  fi
}

# Boolean
runexpr 'true' boolean | outputs TRUE
runexpr 'false' boolean | outputs FALSE
runexpr 'not true' boolean | outputs FALSE
runexpr 'not false' boolean | outputs TRUE
runexpr 'true and true' boolean | outputs TRUE
runexpr 'true and false' boolean | outputs FALSE
runexpr 'false and true' boolean | outputs FALSE
runexpr 'false and false' boolean | outputs FALSE
runexpr 'true or true' boolean | outputs TRUE
runexpr 'true or false' boolean | outputs TRUE
runexpr 'false or true' boolean | outputs TRUE
runexpr 'false or false' boolean | outputs FALSE
runexpr "true = false" boolean | outputs FALSE
runexpr "true > false" boolean | outputs TRUE
runexpr "true < false" boolean | outputs FALSE
runexpr "true >= false" boolean | outputs TRUE
runexpr "true <= false" boolean | outputs FALSE
runexpr "true <> false" boolean | outputs TRUE
runexpr "true = true" boolean | outputs TRUE
runexpr "true > true" boolean | outputs FALSE
runexpr "true < true" boolean | outputs FALSE
runexpr "true >= true" boolean | outputs TRUE
runexpr "true <= true" boolean | outputs TRUE
runexpr "true <> true" boolean | outputs FALSE

# Integer
runexpr '0' integer | outputs 0
runexpr '1' integer | outputs 1
runexpr '-1' integer | outputs -1
runexpr '6 + 2' integer | outputs 8
runexpr '6 - 2' integer | outputs 4
runexpr '6 * 2' integer | outputs 12
runexpr '6 div 2' integer | outputs 3
runexpr '6 mod 2' integer | outputs 0
runexpr '8 div 3' integer | outputs 2
runexpr '8 mod 3' integer | outputs 2
runexpr '5 and 3' integer | outputs 1
runexpr '5 or 3' integer | outputs 7
runexpr "2 = 1" boolean | outputs FALSE
runexpr "2 > 1" boolean | outputs TRUE
runexpr "2 < 1" boolean | outputs FALSE
runexpr "2 >= 1" boolean | outputs TRUE
runexpr "2 <= 1" boolean | outputs FALSE
runexpr "2 <> 1" boolean | outputs TRUE
runexpr "2 = 2" boolean | outputs TRUE
runexpr "2 > 2" boolean | outputs FALSE
runexpr "2 < 2" boolean | outputs FALSE
runexpr "2 >= 2" boolean | outputs TRUE
runexpr "2 <= 2" boolean | outputs TRUE
runexpr "2 <> 2" boolean | outputs FALSE

# Char and string
runexpr "''" char | is_not_valid
runexpr "'a'" char | outputs 'a'
runexpr "'ab'" char | is_not_valid
runexpr "'a' + 'b'" string | outputs 'ab'
runexpr "'a' = 'b'" boolean | outputs FALSE
runexpr "'a' > 'b'" boolean | outputs FALSE
runexpr "'a' < 'b'" boolean | outputs TRUE
runexpr "'a' >= 'b'" boolean | outputs FALSE
runexpr "'a' <= 'b'" boolean | outputs TRUE
runexpr "'a' <> 'b'" boolean | outputs TRUE
runexpr "'a' = 'a'" boolean | outputs TRUE
runexpr "'a' > 'a'" boolean | outputs FALSE
runexpr "'a' < 'a'" boolean | outputs FALSE
runexpr "'a' >= 'a'" boolean | outputs TRUE
runexpr "'a' <= 'a'" boolean | outputs TRUE
runexpr "'a' <> 'a'" boolean | outputs FALSE

runexpr "''" string | outputs ''
runexpr "'a'" string | outputs 'a'
runexpr "'ab'" string | outputs 'ab'
runexpr "'ab' + 'cd'" string | outputs 'abcd'
runexpr "'ab' = 'ba'" boolean | outputs FALSE
runexpr "'ab' > 'ba'" boolean | outputs FALSE
runexpr "'ab' < 'ba'" boolean | outputs TRUE
runexpr "'ab' >= 'ba'" boolean | outputs FALSE
runexpr "'ab' <= 'ba'" boolean | outputs TRUE
runexpr "'ab' <> 'ba'" boolean | outputs TRUE
runexpr "'ab' = 'ab'" boolean | outputs TRUE
runexpr "'ab' > 'ab'" boolean | outputs FALSE
runexpr "'ab' < 'ab'" boolean | outputs FALSE
runexpr "'ab' >= 'ab'" boolean | outputs TRUE
runexpr "'ab' <= 'ab'" boolean | outputs TRUE
runexpr "'ab' <> 'ab'" boolean | outputs FALSE

runexpr "var s:string" "s:='abcde'" "s[1]" char | outputs 'a'
runexpr "var s:string" "s:='abcde'" "s[2]" char | outputs 'b'
runexpr "var s:string" "s:='abcde'" "s[3]" char | outputs 'c'
runexpr "var s:string" "s:='abcde'" "s[4]" char | outputs 'd'
runexpr "var s:string" "s:='abcde'" "s[5]" char | outputs 'e'

# Functions
runexpr "function fun : integer; begin fun := 3 end" "" \
        "fun" integer | outputs 3
runexpr "function fun : integer; begin fun := 3 end" "" \
        "fun()" integer | outputs 3
runexpr "function fun(a : integer) : integer; begin fun := a + 3 end" "" \
        "fun(5)" integer | outputs 8
runexpr "function fun(a : integer; b : integer) : integer;
         begin fun := a + b end" "" \
        "fun(4, 7)" integer | outputs 11
runexpr "function fun(a : integer; b : string) : integer;
         begin fun := a + length(b) end" "" \
        "fun(3, 'hello')" integer | outputs 8

# Operator precedence
runexpr "1 + 2 * 3 + 4" integer | outputs 11
runexpr "(1 + 2) * (3 + 4)" integer | outputs 21
runexpr "1 + 2 and 3 + 4" integer | outputs 7
runexpr "1 * 2 or 3 * 4" integer | outputs 14
runexpr "not false and false" boolean | outputs FALSE
runexpr "(not false) and false" boolean | outputs FALSE
runexpr "not (false and false)" boolean | outputs TRUE
runexpr "not true or true" boolean | outputs TRUE
runexpr "(not true) or true" boolean | outputs TRUE
runexpr "not (true or true)" boolean | outputs FALSE
runexpr "true or true and false" boolean | outputs TRUE
runexpr "true or (true and false)" boolean | outputs TRUE
runexpr "(true or true) and false" boolean | outputs FALSE
runexpr "false = false and false" boolean | outputs TRUE
runexpr "false = (false and false)" boolean | outputs TRUE
runexpr "(false = false) and false" boolean | outputs FALSE
runexpr "false = false or true" boolean | outputs FALSE
runexpr "false = (false or true)" boolean | outputs FALSE
runexpr "(false = false) or true" boolean | outputs TRUE
