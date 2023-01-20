#!/bin/bash

. ./lib.sh

# Minimal program
echo '' | is_not_valid
echo 'program' | is_not_valid
echo 'program;' | is_not_valid
echo 'program.' | is_not_valid
echo 'program foo' | is_not_valid
echo 'program foo;' | is_not_valid
echo 'program foo.' | is_not_valid
echo 'program foo; end' | is_not_valid
echo 'program foo; end;' | is_not_valid
echo 'program foo; end.' | is_not_valid
echo 'program foo; begin' | is_not_valid
echo 'program foo; begin;' | is_not_valid
echo 'program foo; begin.' | is_not_valid
echo 'program foo; begin end' | is_not_valid
echo 'program foo; begin end;' | is_not_valid
echo 'program foo; begin end.' | is_valid
echo 'program; begin end.' | is_not_valid
echo 'program foo; begin writeln(maxint) end.' | is_valid
echo 'program foo; begin writeln(maxint) end;' | is_not_valid

# Input and output files specified in 'program' clause are ignored
echo 'program foo(input); begin writeln(maxint) end.' | is_valid
echo 'program foo(output); begin writeln(maxint) end.' | is_valid
echo 'program foo(input, output); begin writeln(maxint) end.' | is_valid
echo 'program foo(one,two,three,four); begin writeln(maxint) end.' | is_valid

# Identifiers
testid() {
  echo "program foo; var $1 : integer; begin end."
}
testid a | is_valid
testid abcdef | is_valid
testid A | is_valid
testid ABCDEF | is_valid
testid AbcDef | is_valid
testid 1234 | is_not_valid
testid a1234 | is_valid
testid a1b2c3 | is_valid
testid A1234 | is_valid
testid A1b2C3 | is_valid
testid _ | is_valid
testid _abc | is_valid
testid _abc_DEF_123 | is_valid

# Numbers
testinteger() {
  echo "program foo; var a : integer; begin a := $1 end."
}
testinteger 1 | is_valid
testinteger 1234 | is_valid
testinteger -1 | will_be_valid
testinteger -1234 | will_be_valid
testinteger +1 | will_be_valid
testinteger +1234 | will_be_valid
testinteger '$1234' | will_be_valid
testinteger '$CAFE' | will_be_valid
testreal() {
  echo "program foo; var a : real; begin a := $1 end."
}
testreal 1.234 | will_be_valid
testreal -1.234 | will_be_valid
testreal 1.234e3 | will_be_valid
testreal 1.234e-3 | will_be_valid

# Character strings
teststring() {
  echo "program foo; var a : string; begin a := $1 end."
}
teststring "''" | is_valid
teststring '""' | is_not_valid
teststring "'A'" | is_valid
teststring '"A"' | is_not_valid
teststring "'abcdef'" | is_valid
teststring '"abcdef"' | is_not_valid
teststring "'abc''def'" | is_valid
teststring "#65" | will_be_valid
teststring "#65#66#67" | will_be_valid
teststring "'abc'#68'def'" | will_be_valid
teststring "^G" | will_be_valid
teststring "^g" | will_be_valid
teststring "^[" | will_be_valid
teststring "'abc'^G'def'" | will_be_valid
teststring "^G^G^G" | will_be_valid
testchar() {
  echo "program foo; var a : char; begin a := $1 end."
}
testchar "''" | is_not_valid
testchar '""' | is_not_valid
testchar "'a'" | is_valid
testchar '"a"' | is_not_valid
testchar "'ab'" | is_not_valid
testchar '"ab"' | is_not_valid
testchar "#65" | will_be_valid
testchar "^G" | will_be_valid
testchar "^g" | will_be_valid
testchar "^[" | will_be_valid

# Comments
echo '{Short comment}program foo; begin end.' | is_valid
echo '(*Long comment*)program foo; begin end.' | is_valid
echo 'program{comment}foo; begin end.' | is_valid
echo 'program foo{comment}; begin end.' | is_valid
echo 'program foo;{comment}begin end.' | is_valid
echo 'program foo;begin{comment}end.' | is_valid
echo 'program foo;begin end{comment}.' | is_valid
echo 'program foo;begin end.{comment}' | is_valid
echo 'prog{comment}ram foo; begin end.' | is_not_valid

# Blocks
testdef() {
  echo 'program foo; ' ; cat ; echo ' begin end.'
}
echo '' | testdef | is_valid
echo 'const A = 123;' | testdef | is_valid
echo 'type T = integer;' | testdef | is_valid
echo 'var V : boolean;' | testdef | is_valid
echo 'const A = 123; type T = integer;' | testdef | is_valid
echo 'const A = 123; var V : boolean;' | testdef | is_valid
echo 'type T = integer; var V : boolean;' | testdef | is_valid
echo 'const A = 123; type T = integer; var V : boolean;' | testdef | is_valid

# Untyped constant definitions
echo 'const A = 123;' | testdef | is_valid
echo 'const A;' | testdef | is_not_valid
echo 'const A =;' | testdef | is_not_valid
echo 'const A = 123' | testdef | is_not_valid
echo 'const A = 123; B = A;' | testdef | is_valid
echo 'const A = B; B = 123;' | testdef | is_not_valid
echo 'const A = 1 + 2;' | testdef | will_be_valid

# Type definitions
echo 'type A = integer;' | testdef | is_valid
echo 'type A = integer; B = A;' | testdef | is_valid
echo 'type A = B; A = integer;' | testdef | is_not_valid
echo 'type A = integer' | testdef | is_not_valid
echo 'type A=;' | testdef | is_not_valid

# Variable definitions
echo 'var A : integer;' | testdef | is_valid
echo 'type A = integer; var B : A;' | testdef | is_valid
echo 'var A, B : integer;' | testdef | will_be_valid
echo 'var A = boolean;' | testdef | is_not_valid
echo 'var A : boolean' | testdef | is_not_valid
echo 'var A:;' | testdef | is_not_valid

# Type denoters
testtype () {
  echo 'program foo; type T = ' ; cat ; echo '; begin end.'
}
echo 'boolean' | testtype | is_valid
echo 'integer' | testtype | is_valid
echo 'real' | testtype | will_be_valid
echo 'char' | testtype | is_valid
echo 'string' | testtype | is_valid
echo 'totallymadeuptype' | testtype | is_not_valid
echo '(One)' | testtype | is_valid
echo '(One, Two, Three)' | testtype | is_valid
echo '(One, , Two)' | testtype | is_not_valid
echo '1..10' | testtype | will_be_valid
echo "'a'..'z'" | testtype | will_be_valid
echo '(One, Two, Three); X = One..Two' | testtype | will_be_valid
echo 'array[1..10] of integer' | testtype | is_valid
echo 'array[6..15] of integer' | testtype | is_valid
echo 'array[-4..5] of integer' | testtype | will_be_valid
echo 'array[1..10] of array[1..20] of integer' | testtype | is_valid
echo 'array[1..10,1..20] of integer' | testtype | will_be_valid
echo '(One, Two, Three);
      X = array[One..Two] of integer' | testtype | will_be_valid
echo 'array[boolean] of integer' | testtype | will_be_valid
echo 'packed array[1..10] of char' | testtype | will_be_valid
echo 'record A : integer end' | testtype | is_valid
echo 'record A : integer; B : string end' | testtype | is_valid
echo 'record A : integer; B : string; end' | testtype | is_valid
echo 'record A : integer; B : record C : char end end' | testtype | is_valid
echo 'packed record A : integer; B : char end' | testtype | will_be_valid
echo 'record A,B : integer; C : char end' | testtype | will_be_valid
echo 'record A : integer;
             case J : boolean of
                 true : (B : char; C : integer);
                 false : (D : boolean)
      end' | testtype | will_be_valid
echo 'record A : integer;
             case boolean of
                 true : (B : char; C : integer);
                 false : (D : boolean)
      end' | testtype | will_be_valid
echo 'record A : integer;
             case J : boolean of
                 true : (B : char);
                 false : ()
      end' | testtype | will_be_valid
echo '(One, Two, Three);
      X = record A : integer;
                 case J : T of
                   One : (B : char; C : integer);
                   Two : (D : string);
                   Three : (E : char)
          end' | testtype | will_be_valid
echo '(One, Two, Three);
      X = record A : integer;
                 case T of
                   One : (B : char; C : integer);
                   Two : (D : string);
                   Three : (E : char)
          end' | testtype | will_be_valid
echo '(One, Two, Three);
      X = record A : integer;
                 case T of
                   One, Two : (B : char; C : integer);
                   Three : (D : string)
          end' | testtype | will_be_valid
echo '1..5;
      X = record A : integer;
             case T of
               1 : (B : char);
               2, 3 : (C : integer);
               4 : (D : string);
               5 : (E : boolean)
      end' | testtype | will_be_valid
echo 'record A : integer;
             case 1..5 of
               1 : (B : char);
               2, 3 : (C : integer);
               4 : (D : string);
               5 : (E : boolean)
      end' | testtype | is_not_valid
echo 'set of char' | testtype | will_be_valid
echo 'set of (One, Two, Three)' | testtype | will_be_valid
echo '(One, Two, Three); X = set of T' | testtype | will_be_valid
echo 'set of integer' | testtype | is_not_valid
echo 'set of 5..15' | testtype | will_be_valid
echo "set of 'a'..'z'" | testtype | will_be_valid
echo 'file of integer' | testtype | will_be_valid
echo 'file of record A : integer; B : char end' | testtype | is_not_valid
echo 'record A : integer; B : char end; X = file of T' | testtype | will_be_valid
echo 'text' | testtype | is_valid
echo '^integer' | testtype | is_valid
echo 'integer; X = ^T' | testtype | is_valid
echo '^integer; X = T' | testtype | is_valid
echo '^integer; X = ^T' | testtype | is_valid
echo '^X; X = record A : integer end' | testtype | will_be_valid
echo '^X; X = integer' | testtype | will_be_valid
