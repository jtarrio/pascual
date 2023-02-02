#!/bin/bash

. ./testing.libsh

# testimm expr rettype
testimm() {
  echo "program foo; var _res : $2; begin _res := $1; write(_res) end."
}

# testvar argtype arg expr rettype
# testvar argtype arg1 arg2 expr rettype
testvar() {
  if [ -z "$5" ]
  then
    echo "program foo;
            type t = $1;
            var a : t;
                _res : $4;
            begin
            a := $2; _res := $3;
            write(_res)
            end."
  else
    echo "program foo;
            type t = $1;
            var a, b : t;
                _res : $5;
            begin
            a := $2; b := $3; _res := $4;
            write(_res)
            end."
  fi
}

# Boolean
testimm 'true' boolean | outputs TRUE
testimm 'false' boolean | outputs FALSE
testimm 'not true' boolean | outputs FALSE
testimm 'not false' boolean | outputs TRUE
testimm 'true and true' boolean | outputs TRUE
testimm 'true and false' boolean | outputs FALSE
testimm 'false and true' boolean | outputs FALSE
testimm 'false and false' boolean | outputs FALSE
testimm 'true or true' boolean | outputs TRUE
testimm 'true or false' boolean | outputs TRUE
testimm 'false or true' boolean | outputs TRUE
testimm 'false or false' boolean | outputs FALSE
testimm "true = false" boolean | outputs FALSE
testimm "true > false" boolean | outputs TRUE
testimm "true < false" boolean | outputs FALSE
testimm "true >= false" boolean | outputs TRUE
testimm "true <= false" boolean | outputs FALSE
testimm "true <> false" boolean | outputs TRUE
testimm "true = true" boolean | outputs TRUE
testimm "true > true" boolean | outputs FALSE
testimm "true < true" boolean | outputs FALSE
testimm "true >= true" boolean | outputs TRUE
testimm "true <= true" boolean | outputs TRUE
testimm "true <> true" boolean | outputs FALSE
testimm "Ord(true)" integer | outputs 1
testimm "Ord(false)" integer | outputs 0
testimm "Pred(true)" boolean | outputs FALSE
testimm "Pred(false)" boolean | is_not_valid
testimm "Succ(true)" boolean | is_not_valid
testimm "Succ(false)" boolean | outputs TRUE

testvar boolean 'true' 'a' boolean | outputs TRUE
testvar boolean 'false' 'a' boolean | outputs FALSE
testvar boolean 'true' 'not a' boolean | outputs FALSE
testvar boolean 'false' 'not a' boolean | outputs TRUE
testvar boolean 'true' 'true' 'a and b' boolean | outputs TRUE
testvar boolean 'true' 'false' 'a and b' boolean | outputs FALSE
testvar boolean 'false' 'true' 'a and b' boolean | outputs FALSE
testvar boolean 'false' 'false' 'a and b' boolean | outputs FALSE
testvar boolean 'true' 'true' 'a or b' boolean | outputs TRUE
testvar boolean 'true' 'false' 'a or b' boolean | outputs TRUE
testvar boolean 'false' 'true' 'a or b' boolean | outputs TRUE
testvar boolean 'false' 'false' 'a or b' boolean | outputs FALSE
testvar boolean 'true' 'false' 'a = b' boolean | outputs FALSE
testvar boolean 'true' 'false' 'a > b' boolean | outputs TRUE
testvar boolean 'true' 'false' 'a < b' boolean | outputs FALSE
testvar boolean 'true' 'false' 'a >= b' boolean | outputs TRUE
testvar boolean 'true' 'false' 'a <= b' boolean | outputs FALSE
testvar boolean 'true' 'false' 'a <> b' boolean | outputs TRUE
testvar boolean 'true' 'true' 'a = b' boolean | outputs TRUE
testvar boolean 'true' 'true' 'a > b' boolean | outputs FALSE
testvar boolean 'true' 'true' 'a < b' boolean | outputs FALSE
testvar boolean 'true' 'true' 'a >= b' boolean | outputs TRUE
testvar boolean 'true' 'true' 'a <= b' boolean | outputs TRUE
testvar boolean 'true' 'true' 'a <> b' boolean | outputs FALSE
testvar boolean 'true' 'Ord(a)' integer | outputs 1
testvar boolean 'false' 'Ord(a)' integer | outputs 0
testvar boolean 'true' 'Pred(a)' boolean | outputs FALSE
testvar boolean 'false' 'Succ(a)' boolean | outputs TRUE

# Integer
testimm '0' integer | outputs 0
testimm '1' integer | outputs 1
testimm '-1' integer | outputs -1
testimm '6 + 2' integer | outputs 8
testimm '6 - 2' integer | outputs 4
testimm '6 * 2' integer | outputs 12
testimm '6 div 2' integer | outputs 3
testimm '6 mod 2' integer | outputs 0
testimm '8 div 3' integer | outputs 2
testimm '8 mod 3' integer | outputs 2
testimm '5 and 3' integer | outputs 1
testimm '5 or 3' integer | outputs 7
testimm "2 = 1" boolean | outputs FALSE
testimm "2 > 1" boolean | outputs TRUE
testimm "2 < 1" boolean | outputs FALSE
testimm "2 >= 1" boolean | outputs TRUE
testimm "2 <= 1" boolean | outputs FALSE
testimm "2 <> 1" boolean | outputs TRUE
testimm "2 = 2" boolean | outputs TRUE
testimm "2 > 2" boolean | outputs FALSE
testimm "2 < 2" boolean | outputs FALSE
testimm "2 >= 2" boolean | outputs TRUE
testimm "2 <= 2" boolean | outputs TRUE
testimm "2 <> 2" boolean | outputs FALSE
testimm "Ord(1)" integer | outputs 1
testimm "Ord(2)" integer | outputs 2
testimm "Pred(1)" integer | outputs 0
testimm "Pred(2)" integer | outputs 1
testimm "Succ(1)" integer | outputs 2
testimm "Succ(2)" integer | outputs 3

testvar integer '0' 'a' integer | outputs 0
testvar integer '1' 'a' integer | outputs 1
testvar integer '-1' 'a' integer | outputs -1
testvar integer '6' '2' 'a + b' integer | outputs 8
testvar integer '6' '2' 'a - b' integer | outputs 4
testvar integer '6' '2' 'a * b' integer | outputs 12
testvar integer '6' '2' 'a div b' integer | outputs 3
testvar integer '6' '2' 'a mod b' integer | outputs 0
testvar integer '8' '3' 'a div b' integer | outputs 2
testvar integer '8' '3' 'a mod b' integer | outputs 2
testvar integer '5' '3' 'a and b' integer | outputs 1
testvar integer '5' '3' 'a or b' integer | outputs 7
testvar integer '2' '1' 'a = b' boolean | outputs FALSE
testvar integer '2' '1' 'a > b' boolean | outputs TRUE
testvar integer '2' '1' 'a < b' boolean | outputs FALSE
testvar integer '2' '1' 'a >= b' boolean | outputs TRUE
testvar integer '2' '1' 'a <= b' boolean | outputs FALSE
testvar integer '2' '1' 'a <> b' boolean | outputs TRUE
testvar integer '2' '2' 'a = b' boolean | outputs TRUE
testvar integer '2' '2' 'a > b' boolean | outputs FALSE
testvar integer '2' '2' 'a < b' boolean | outputs FALSE
testvar integer '2' '2' 'a >= b' boolean | outputs TRUE
testvar integer '2' '2' 'a <= b' boolean | outputs TRUE
testvar integer '2' '2' 'a <> b' boolean | outputs FALSE
testvar integer '1' 'Ord(a)' integer | outputs 1
testvar integer '2' 'Ord(a)' integer | outputs 2
testvar integer '1' 'Pred(a)' integer | outputs 0
testvar integer '2' 'Pred(a)' integer | outputs 1
testvar integer '1' 'Succ(a)' integer | outputs 2
testvar integer '2' 'Succ(a)' integer | outputs 3

# Char and string
testimm "''" char | is_not_valid
testimm "'a'" char | outputs 'a'
testimm "'ab'" char | is_not_valid
testimm "'a' + 'b'" string | outputs 'ab'
testimm "'a' = 'b'" boolean | outputs FALSE
testimm "'a' > 'b'" boolean | outputs FALSE
testimm "'a' < 'b'" boolean | outputs TRUE
testimm "'a' >= 'b'" boolean | outputs FALSE
testimm "'a' <= 'b'" boolean | outputs TRUE
testimm "'a' <> 'b'" boolean | outputs TRUE
testimm "'a' = 'a'" boolean | outputs TRUE
testimm "'a' > 'a'" boolean | outputs FALSE
testimm "'a' < 'a'" boolean | outputs FALSE
testimm "'a' >= 'a'" boolean | outputs TRUE
testimm "'a' <= 'a'" boolean | outputs TRUE
testimm "'a' <> 'a'" boolean | outputs FALSE
testimm "Ord('a')" integer | outputs 97
testimm "Ord('b')" integer | outputs 98
testimm "Pred('a')" char | outputs '`'
testimm "Pred('b')" char | outputs 'a'
testimm "Succ('a')" char | outputs 'b'
testimm "Succ('b')" char | outputs 'c'

testimm "''" string | outputs ''
testimm "'a'" string | outputs 'a'
testimm "'ab'" string | outputs 'ab'
testimm "'ab' + 'cd'" string | outputs 'abcd'
testimm "'ab' = 'ba'" boolean | outputs FALSE
testimm "'ab' > 'ba'" boolean | outputs FALSE
testimm "'ab' < 'ba'" boolean | outputs TRUE
testimm "'ab' >= 'ba'" boolean | outputs FALSE
testimm "'ab' <= 'ba'" boolean | outputs TRUE
testimm "'ab' <> 'ba'" boolean | outputs TRUE
testimm "'ab' = 'ab'" boolean | outputs TRUE
testimm "'ab' > 'ab'" boolean | outputs FALSE
testimm "'ab' < 'ab'" boolean | outputs FALSE
testimm "'ab' >= 'ab'" boolean | outputs TRUE
testimm "'ab' <= 'ab'" boolean | outputs TRUE
testimm "'ab' <> 'ab'" boolean | outputs FALSE

testvar char "''" 'a' string | is_not_valid
testvar char "'a'" 'a' string | outputs 'a'
testvar char "'ab'" 'a' string | is_not_valid
testvar char "'a'" "'b'" 'a + b' string | outputs 'ab'
testvar char "'a'" "'b'" 'a = b' boolean | outputs FALSE
testvar char "'a'" "'b'" 'a > b' boolean | outputs FALSE
testvar char "'a'" "'b'" 'a < b' boolean | outputs TRUE
testvar char "'a'" "'b'" 'a >= b' boolean | outputs FALSE
testvar char "'a'" "'b'" 'a <= b' boolean | outputs TRUE
testvar char "'a'" "'b'" 'a <> b' boolean | outputs TRUE
testvar char "'a'" "'a'" 'a = b' boolean | outputs TRUE
testvar char "'a'" "'a'" 'a > b' boolean | outputs FALSE
testvar char "'a'" "'a'" 'a < b' boolean | outputs FALSE
testvar char "'a'" "'a'" 'a >= b' boolean | outputs TRUE
testvar char "'a'" "'a'" 'a <= b' boolean | outputs TRUE
testvar char "'a'" "'a'" 'a <> b' boolean | outputs FALSE
testvar char "'a'" 'Ord(a)' integer | outputs 97
testvar char "'b'" 'Ord(a)' integer | outputs 98
testvar char "'a'" 'Pred(a)' char | outputs '`'
testvar char "'b'" 'Pred(a)' char | outputs 'a'
testvar char "'a'" 'Succ(a)' char | outputs 'b'
testvar char "'b'" 'Succ(a)' char | outputs 'c'

testvar string "''" 'a' string | outputs ''
testvar string "'a'" 'a' string | outputs 'a'
testvar string "'ab'" 'a' string | outputs 'ab'
testvar string "'ab'" "'cd'" 'a + b' string | outputs 'abcd'
testvar string "'ab'" "'ba'" 'a = b' boolean | outputs FALSE
testvar string "'ab'" "'ba'" 'a > b' boolean | outputs FALSE
testvar string "'ab'" "'ba'" 'a < b' boolean | outputs TRUE
testvar string "'ab'" "'ba'" 'a >= b' boolean | outputs FALSE
testvar string "'ab'" "'ba'" 'a <= b' boolean | outputs TRUE
testvar string "'ab'" "'ba'" 'a <> b' boolean | outputs TRUE
testvar string "'ab'" "'ab'" 'a = b' boolean | outputs TRUE
testvar string "'ab'" "'ab'" 'a > b' boolean | outputs FALSE
testvar string "'ab'" "'ab'" 'a < b' boolean | outputs FALSE
testvar string "'ab'" "'ab'" 'a >= b' boolean | outputs TRUE
testvar string "'ab'" "'ab'" 'a <= b' boolean | outputs TRUE
testvar string "'ab'" "'ab'" 'a <> b' boolean | outputs FALSE

testvar string "'abcde'" "''" 'a[1]' char | outputs 'a'
testvar string "'abcde'" "''" 'a[2]' char | outputs 'b'
testvar string "'abcde'" "''" 'a[3]' char | outputs 'c'
testvar string "'abcde'" "''" 'a[4]' char | outputs 'd'
testvar string "'abcde'" "''" 'a[5]' char | outputs 'e'

# Enumerated types
# testenum typedef arg argtype expr rettype
testenum() {
  echo "program foo;
        type t = $1;
        var a : $3; _ret : $5;
        begin a := $2; _ret := $4; write(_ret) end."
}

testimm 'One' '(ONE, TWO, THREE)' | outputs 'ONE'
testimm 'Two' '(ONE, TWO, THREE)' | outputs 'TWO'
testimm 'Three' '(ONE, TWO, THREE)' | outputs 'THREE'
testenum '(ONE, TWO, THREE)' 'One = Two' boolean 'a' boolean | outputs FALSE
testenum '(ONE, TWO, THREE)' 'One <> Two' boolean 'a' boolean | outputs TRUE
testenum '(ONE, TWO, THREE)' 'One > Two' boolean 'a' boolean | outputs FALSE
testenum '(ONE, TWO, THREE)' 'One < Two' boolean 'a' boolean | outputs TRUE
testenum '(ONE, TWO, THREE)' 'One >= Two' boolean 'a' boolean | outputs FALSE
testenum '(ONE, TWO, THREE)' 'One <= Two' boolean 'a' boolean | outputs TRUE
testenum '(ONE, TWO, THREE)' 'Two = Two' boolean 'a' boolean | outputs TRUE
testenum '(ONE, TWO, THREE)' 'Two <> Two' boolean 'a' boolean | outputs FALSE
testenum '(ONE, TWO, THREE)' 'Two > Two' boolean 'a' boolean | outputs FALSE
testenum '(ONE, TWO, THREE)' 'Two < Two' boolean 'a' boolean | outputs FALSE
testenum '(ONE, TWO, THREE)' 'Two >= Two' boolean 'a' boolean | outputs TRUE
testenum '(ONE, TWO, THREE)' 'Two <= Two' boolean 'a' boolean | outputs TRUE

testvar '(ONE, TWO, THREE)' 'One' 'a' t | outputs 'ONE'
testvar '(ONE, TWO, THREE)' 'Two' 'a' t | outputs 'TWO'
testvar '(ONE, TWO, THREE)' 'Three' 'a' t | outputs 'THREE'
testvar '(ONE, TWO, THREE)' 'One' 'Two' 'a = b' boolean | outputs FALSE
testvar '(ONE, TWO, THREE)' 'One' 'Two' 'a <> b' boolean | outputs TRUE
testvar '(ONE, TWO, THREE)' 'One' 'Two' 'a > b' boolean | outputs FALSE
testvar '(ONE, TWO, THREE)' 'One' 'Two' 'a < b' boolean | outputs TRUE
testvar '(ONE, TWO, THREE)' 'One' 'Two' 'a >= b' boolean | outputs FALSE
testvar '(ONE, TWO, THREE)' 'One' 'Two' 'a <= b' boolean | outputs TRUE
testvar '(ONE, TWO, THREE)' 'Two' 'Two' 'a = b' boolean | outputs TRUE
testvar '(ONE, TWO, THREE)' 'Two' 'Two' 'a <> b' boolean | outputs FALSE
testvar '(ONE, TWO, THREE)' 'Two' 'Two' 'a > b' boolean | outputs FALSE
testvar '(ONE, TWO, THREE)' 'Two' 'Two' 'a < b' boolean | outputs FALSE
testvar '(ONE, TWO, THREE)' 'Two' 'Two' 'a >= b' boolean | outputs TRUE
testvar '(ONE, TWO, THREE)' 'Two' 'Two' 'a <= b' boolean | outputs TRUE

testenum '(ONE, TWO, THREE)' 'Ord(One)' integer 'a' integer | outputs '0'
testenum '(ONE, TWO, THREE)' 'Ord(Two)' integer 'a' integer | outputs '1'
testenum '(ONE, TWO, THREE)' 'Ord(Three)' integer 'a' integer | outputs '2'
testenum '(ONE, TWO, THREE)' 'Succ(One)' t 'a' t | outputs 'TWO'
testenum '(ONE, TWO, THREE)' 'Succ(Two)' t 'a' t | outputs 'THREE'
testenum '(ONE, TWO, THREE)' 'Succ(Three)' t 'a' t | is_not_valid
testenum '(ONE, TWO, THREE)' 'Pred(One)' t 'a' t | is_not_valid
testenum '(ONE, TWO, THREE)' 'Pred(Two)' t 'a' t | outputs 'ONE'
testenum '(ONE, TWO, THREE)' 'Pred(Three)' t 'a' t | outputs 'TWO'
testvar '(ONE, TWO, THREE)' 'One' 'Ord(a)' integer | outputs '0'
testvar '(ONE, TWO, THREE)' 'Two' 'Ord(a)' integer | outputs '1'
testvar '(ONE, TWO, THREE)' 'Three' 'Ord(a)' integer | outputs '2'
testvar '(ONE, TWO, THREE)' 'One' 'Succ(a)' t | outputs 'TWO'
testvar '(ONE, TWO, THREE)' 'Two' 'Succ(a)' t | outputs 'THREE'
testvar '(ONE, TWO, THREE)' 'Two' 'Pred(a)' t | outputs 'ONE'
testvar '(ONE, TWO, THREE)' 'Three' 'Pred(a)' t | outputs 'TWO'

# Functions
testfun() {
  echo "program foo; var _ret : $3; $1; begin write($2) end."
}
testfun "function fun : integer; begin fun := 3 end" \
        "fun" integer | outputs 3
testfun "function fun : integer; begin fun := 3 end" \
        "fun()" integer | outputs 3
testfun "function fun(a : integer) : integer; begin fun := a + 3 end" \
        "fun(5)" integer | outputs 8
testfun "function fun(a : integer; b : integer) : integer;
         begin fun := a + b end" \
        "fun(4, 7)" integer | outputs 11
testfun "function fun(a : integer; b : string) : integer;
         begin fun := a + length(b) end" \
        "fun(3, 'hello')" integer | outputs 8

# Operator precedence
testimm "1 + 2 * 3 + 4" integer | outputs 11
testimm "(1 + 2) * (3 + 4)" integer | outputs 21
testimm "1 + 2 and 3 + 4" integer | outputs 7
testimm "1 + (2 and 3) + 4" integer | outputs 7
testimm "(1 + 2) and (3 + 4)" integer | outputs 3
testimm "1 * 2 or 3 * 4" integer | outputs 14
testimm "(1 * 2) or (3 * 4)" integer | outputs 14
testimm "1 * (2 or 3) * 4" integer | outputs 12
testimm "not false and false" boolean | outputs FALSE
testimm "(not false) and false" boolean | outputs FALSE
testimm "not (false and false)" boolean | outputs TRUE
testimm "not true or true" boolean | outputs TRUE
testimm "(not true) or true" boolean | outputs TRUE
testimm "not (true or true)" boolean | outputs FALSE
testimm "true or true and false" boolean | outputs TRUE
testimm "true or (true and false)" boolean | outputs TRUE
testimm "(true or true) and false" boolean | outputs FALSE
testimm "false = false and false" boolean | outputs TRUE
testimm "false = (false and false)" boolean | outputs TRUE
testimm "(false = false) and false" boolean | outputs FALSE
testimm "false = false or true" boolean | outputs FALSE
testimm "false = (false or true)" boolean | outputs FALSE
testimm "(false = false) or true" boolean | outputs TRUE
