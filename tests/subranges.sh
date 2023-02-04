#!/bin/bash

. ./testing.libsh

# testrange argtype arg expr
# testrange argtype arg1 arg2 expr
testrange() {
  if [ -z "$4" ]
  then
    echo "{\$R+}
          program foo;
          type t = $1;
          var arg : t;
          begin
          arg := $2;
          write($3)
          end."
  else
    echo "{\$R+}
          program foo;
          type t = $1;
          var arg1, arg2 : t;
          begin
          arg1 := $2; arg2 := $3;
          write($4)
          end."
  fi
}

# testenumrange enum argtype arg expr
# testenumrange enum argtype arg1 arg2 expr
testenumrange() {
  if [ -z "$5" ]
  then
    echo "{\$R+}
          program foo;
          type te = $1; t = $2;
          var arg : t;
          begin
          arg := $3;
          write($4)
          end."
  else
    echo "{\$R+}
          program foo;
          type te = $1; t = $2;
          var arg1, arg2 : t;
          begin
          arg1 := $3; arg2 := $4;
          write($5)
          end."
  fi
}

# Range of integer
testrange '91..100' '90' arg | is_not_valid
testrange '91..100' '91' arg | outputs 91
testrange '91..100' '100' arg | outputs 100
testrange '91..100' '101' arg | is_not_valid
testrange '91..100' '91' 'ord(arg)' | outputs 91
testrange '91..100' '100' 'ord(arg)' | outputs 100
testrange '91..100' '91' 'pred(arg)' | aborts
testrange '91..100' '92' 'pred(arg)' | outputs 91
testrange '91..100' '99' 'succ(arg)' | outputs 100
testrange '91..100' '100' 'succ(arg)' | aborts
testrange '91..100' '91' 'arg1 - 1' arg2 | aborts
testrange '91..100' '92' 'arg1 - 1' arg2 | outputs 91
testrange '91..100' '99' 'arg1 + 1' arg2 | outputs 100
testrange '91..100' '100' 'arg1 + 1' arg2 | aborts

testrange '91..100' '91' '91' 'arg1 = arg2' | outputs TRUE
testrange '91..100' '91' '91' 'arg1 <> arg2' | outputs FALSE
testrange '91..100' '91' '91' 'arg1 > arg2' | outputs FALSE
testrange '91..100' '91' '91' 'arg1 < arg2' | outputs FALSE
testrange '91..100' '91' '91' 'arg1 >= arg2' | outputs TRUE
testrange '91..100' '91' '91' 'arg1 <= arg2' | outputs TRUE
testrange '91..100' '91' '92' 'arg1 = arg2' | outputs FALSE
testrange '91..100' '91' '92' 'arg1 <> arg2' | outputs TRUE
testrange '91..100' '91' '92' 'arg1 > arg2' | outputs FALSE
testrange '91..100' '91' '92' 'arg1 < arg2' | outputs TRUE
testrange '91..100' '91' '92' 'arg1 >= arg2' | outputs FALSE
testrange '91..100' '91' '92' 'arg1 <= arg2' | outputs TRUE

# Range of char
testrange "'a'..'z'" "'a'" arg | outputs 'a'
testrange "'a'..'z'" "'z'" arg | outputs 'z'
testrange "'a'..'z'" "'A'" arg | is_not_valid
testrange "'a'..'z'" "'a'" "ord(arg)" | outputs 97
testrange "'a'..'z'" "'z'" "ord(arg)" | outputs 122
testrange "'a'..'z'" "'a'" "pred(arg)" | aborts
testrange "'a'..'z'" "'z'" "pred(arg)" | outputs 'y'
testrange "'a'..'z'" "'a'" "succ(arg)" | outputs 'b'
testrange "'a'..'z'" "'z'" "succ(arg)" | aborts

testrange "'a'..'z'" "'a'" "'a'" 'arg1 = arg2' | outputs TRUE
testrange "'a'..'z'" "'a'" "'a'" 'arg1 <> arg2' | outputs FALSE
testrange "'a'..'z'" "'a'" "'a'" 'arg1 > arg2' | outputs FALSE
testrange "'a'..'z'" "'a'" "'a'" 'arg1 < arg2' | outputs FALSE
testrange "'a'..'z'" "'a'" "'a'" 'arg1 >= arg2' | outputs TRUE
testrange "'a'..'z'" "'a'" "'a'" 'arg1 <= arg2' | outputs TRUE
testrange "'a'..'z'" "'a'" "'b'" 'arg1 = arg2' | outputs FALSE
testrange "'a'..'z'" "'a'" "'b'" 'arg1 <> arg2' | outputs TRUE
testrange "'a'..'z'" "'a'" "'b'" 'arg1 > arg2' | outputs FALSE
testrange "'a'..'z'" "'a'" "'b'" 'arg1 < arg2' | outputs TRUE
testrange "'a'..'z'" "'a'" "'b'" 'arg1 >= arg2' | outputs FALSE
testrange "'a'..'z'" "'a'" "'b'" 'arg1 <= arg2' | outputs TRUE

# Range of enumeration
testenumrange '(A, B, C, D, E, F)' 'B..E' 'A' arg | is_not_valid
testenumrange '(A, B, C, D, E, F)' 'B..E' 'B' arg | outputs B
testenumrange '(A, B, C, D, E, F)' 'B..E' 'E' arg | outputs E
testenumrange '(A, B, C, D, E, F)' 'B..E' 'F' arg | is_not_valid
testenumrange '(A, B, C, D, E, F)' 'B..E' 'B' "ord(arg)" | outputs 1
testenumrange '(A, B, C, D, E, F)' 'B..E' 'E' "ord(arg)" | outputs 4
testenumrange '(A, B, C, D, E, F)' 'B..E' 'B' "pred(arg)" | aborts
testenumrange '(A, B, C, D, E, F)' 'B..E' 'E' "pred(arg)" | outputs D
testenumrange '(A, B, C, D, E, F)' 'B..E' 'B' "succ(arg)" | outputs C
testenumrange '(A, B, C, D, E, F)' 'B..E' 'E' "succ(arg)" | aborts

testenumrange '(A, B, C, D, E, F)' 'B..E' 'B' 'B' 'arg1 = arg2' | outputs TRUE
testenumrange '(A, B, C, D, E, F)' 'B..E' 'B' 'B' 'arg1 <> arg2' | outputs FALSE
testenumrange '(A, B, C, D, E, F)' 'B..E' 'B' 'B' 'arg1 > arg2' | outputs FALSE
testenumrange '(A, B, C, D, E, F)' 'B..E' 'B' 'B' 'arg1 < arg2' | outputs FALSE
testenumrange '(A, B, C, D, E, F)' 'B..E' 'B' 'B' 'arg1 >= arg2' | outputs TRUE
testenumrange '(A, B, C, D, E, F)' 'B..E' 'B' 'B' 'arg1 <= arg2' | outputs TRUE
testenumrange '(A, B, C, D, E, F)' 'B..E' 'B' 'C' 'arg1 = arg2' | outputs FALSE
testenumrange '(A, B, C, D, E, F)' 'B..E' 'B' 'C' 'arg1 <> arg2' | outputs TRUE
testenumrange '(A, B, C, D, E, F)' 'B..E' 'B' 'C' 'arg1 > arg2' | outputs FALSE
testenumrange '(A, B, C, D, E, F)' 'B..E' 'B' 'C' 'arg1 < arg2' | outputs TRUE
testenumrange '(A, B, C, D, E, F)' 'B..E' 'B' 'C' 'arg1 >= arg2' | outputs FALSE
testenumrange '(A, B, C, D, E, F)' 'B..E' 'B' 'C' 'arg1 <= arg2' | outputs TRUE

# Range of string is not valid
testrange "'ab'..'cd'" "'ab'" arg | is_not_valid
# Range of boolean is valid though not extremely useful? Maybe?
testrange 'false..true' 'false' arg | outputs FALSE
testrange 'false..true' 'true' arg | outputs TRUE
testrange 'false..true' 'succ(false)' arg | outputs TRUE
testrange 'false..true' 'succ(true)' arg | is_not_valid
testrange 'false..true' 'pred(false)' arg | is_not_valid
testrange 'false..true' 'pred(true)' arg | outputs FALSE
testrange 'true..true' 'false' arg | is_not_valid
testrange 'false..false' 'true' arg | is_not_valid
# The bounds of a range must be in ascending order
testrange '1..2' '1' arg | outputs 1
testrange '1..1' '1' arg | outputs 1
testrange '2..1' '1' arg | is_not_valid
