#!/bin/bash

. ./testing.libsh

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
  echo "program foo; const a : integer = $1; begin end."
}
testinteger 1 | is_valid
testinteger 1234 | is_valid
testinteger -1 | is_valid
testinteger -1234 | is_valid
testinteger +1 | is_valid
testinteger +1234 | is_valid
testinteger '$1234' | is_valid
testinteger '$CAFE' | is_valid
testreal() {
  echo "program foo; const a : real = $1; begin end."
}
testreal 1.234 | is_valid
testreal -1.234 | is_valid
testreal 1e3 | is_valid
testreal 1e-3 | is_valid
testreal 1.234e3 | is_valid
testreal 1.234e-3 | is_valid

# Character strings
teststring() {
  echo "program foo; const a : string = $1; begin end."
}
teststring "''" | is_valid
teststring '""' | is_not_valid
teststring "'A'" | is_valid
teststring '"A"' | is_not_valid
teststring "'abcdef'" | is_valid
teststring '"abcdef"' | is_not_valid
teststring "'abc''def'" | is_valid
teststring "#65" | is_valid
teststring "#65#66#67" | is_valid
teststring "'abc'#68'def'" | is_valid
teststring "'abc'^G'def'" | is_valid
teststring "''^G" | is_valid
teststring "''^g" | is_valid
teststring "''^[" | is_valid
teststring "''^G^G^G" | is_valid
testchar() {
  echo "program foo; var a : char; begin a := $1 end."
}
testchar "''" | is_not_valid
testchar '""' | is_not_valid
testchar "'a'" | is_valid
testchar '"a"' | is_not_valid
testchar "'ab'" | is_not_valid
testchar '"ab"' | is_not_valid
testchar "#65" | is_valid
testchar "''^G" | is_valid
testchar "''^g" | is_valid
testchar "''^[" | is_valid

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
echo 'const C : integer = 123;' | testdef | is_valid
echo 'type T = integer;' | testdef | is_valid
echo 'var V : boolean;' | testdef | is_valid
echo 'const A = 123;
      const C : integer = 456;
      type T = integer;
      var V : boolean;' | testdef | is_valid

# Untyped constant definitions
echo 'const A = 123;' | testdef | is_valid
echo 'const A;' | testdef | is_not_valid
echo 'const A =;' | testdef | is_not_valid
echo 'const A = 123' | testdef | is_not_valid
echo 'const A = 123; B = A;' | testdef | is_valid
echo 'const A = B; B = 123;' | testdef | is_not_valid
echo 'const A = 1 + 2;' | testdef | is_valid

# Typed constant definitions
echo 'const A : integer = 123;' | testdef | is_valid
echo 'const A : integer;' | testdef | is_not_valid
echo 'const A : integer = 123; B : integer = A;' | testdef | is_not_valid
echo 'const A : integer = 1 + 2;' | testdef | is_valid
echo 'const A : array[1..5] of integer = (1, 2, 3, 4, 5);' | testdef | is_valid
echo 'const A : array[1..5] of integer = 5;' | testdef | is_not_valid

# Type definitions
echo 'type A = integer;' | testdef | is_valid
echo 'type A = integer; B = A;' | testdef | is_valid
echo 'type A = B; A = integer;' | testdef | is_not_valid
echo 'type A = integer' | testdef | is_not_valid
echo 'type A=;' | testdef | is_not_valid

# Variable definitions
echo 'var A : integer;' | testdef | is_valid
echo 'type A = integer; var B : A;' | testdef | is_valid
echo 'var A, B : integer;' | testdef | is_valid
echo 'var A = boolean;' | testdef | is_not_valid
echo 'var A : boolean' | testdef | is_not_valid
echo 'var A:;' | testdef | is_not_valid

# Type denoters
testtype() {
  echo 'program foo; type T = ' ; cat ; echo '; begin end.'
}
echo 'boolean' | testtype | is_valid
echo 'integer' | testtype | is_valid
echo 'real' | testtype | is_valid
echo 'char' | testtype | is_valid
echo 'string' | testtype | is_valid
echo 'totallymadeuptype' | testtype | is_not_valid
echo '(One)' | testtype | is_valid
echo '(One, Two, Three)' | testtype | is_valid
echo '(One, , Two)' | testtype | is_not_valid
echo '1..10' | testtype | is_valid
echo "'a'..'z'" | testtype | is_valid
echo '(One, Two, Three); X = One..Two' | testtype | is_valid
echo 'array[1..10] of integer' | testtype | is_valid
echo 'array[6..15] of integer' | testtype | is_valid
echo 'array[-4..5] of integer' | testtype | is_valid
echo 'array[1..10] of array[1..20] of integer' | testtype | is_valid
echo 'array[1..10,1..20] of integer' | testtype | is_valid
echo '(One, Two, Three);
      X = array[One..Two] of integer' | testtype | is_valid
echo 'array[boolean] of integer' | testtype | is_valid
echo 'packed array[1..10] of char' | testtype | will_be_valid
echo 'record A : integer end' | testtype | is_valid
echo 'record A : integer; B : string end' | testtype | is_valid
echo 'record A : integer; B : string; end' | testtype | is_valid
echo 'record A : integer; A : string end' | testtype | is_not_valid
echo 'record A : integer; B : record C : char end end' | testtype | is_valid
echo 'packed record A : integer; B : char end' | testtype | will_be_valid
echo 'record A,B : integer; C : char end' | testtype | is_valid
echo 'record A : integer;
             case J : boolean of
                 true : (B : char; C : integer);
                 false : (D : boolean)
      end' | testtype | is_valid
echo 'record A : integer;
             case J : boolean of
                 true : (A : char; B : integer);
                 false : (C : boolean)
      end' | testtype | is_not_valid
echo 'record case J : boolean of 
                 true : (B : char; C : integer);
                 false : (D : boolean)
      end' | testtype | is_valid
echo 'record A : integer;
             case boolean of
                 true : (B : char; C : integer);
                 false : (D : boolean)
      end' | testtype | is_valid
echo 'record A : integer;
             case J : boolean of
                 true : (B : char);
                 false : ()
      end' | testtype | is_valid
echo '(One, Two, Three);
      X = record A : integer;
                 case J : T of
                   One : (B : char; C : integer);
                   Two : (D : string);
                   Three : (E : char)
          end' | testtype | is_valid
echo '(One, Two, Three);
      X = record A : integer;
                 case T of
                   One : (B : char; C : integer);
                   Two : (D : string);
                   Three : (E : char)
          end' | testtype | is_valid
echo '(One, Two, Three);
      X = record A : integer;
                 case T of
                   One, Two : (B : char; C : integer);
                   Three : (D : string)
          end' | testtype | is_valid
echo '1..5;
      X = record A : integer;
             case T of
               1 : (B : char);
               2, 3 : (C : integer);
               4 : (D : string);
               5 : (E : boolean)
      end' | testtype | is_valid
echo 'record A : integer;
             case 1..5 of
               1 : (B : char);
               2, 3 : (C : integer);
               4 : (D : string);
               5 : (E : boolean)
      end' | testtype | is_not_valid
echo "record A : integer;
             case string of
               'foo' : (B : char)
      end" | testtype | is_not_valid
echo 'set of char' | testtype | is_valid
echo 'set of (One, Two, Three)' | testtype | is_valid
echo '(One, Two, Three); X = set of T' | testtype | is_valid
echo 'set of integer' | testtype | is_not_valid
echo 'set of 5..15' | testtype | is_valid
echo "set of 'a'..'z'" | testtype | is_valid
echo 'file of integer' | testtype | will_be_valid
echo 'file of record A : integer; B : char end' | testtype | is_not_valid
echo 'record A : integer; B : char end; X = file of T' | testtype | will_be_valid
echo 'text' | testtype | is_valid
echo '^integer' | testtype | is_valid
echo 'integer; X = ^T' | testtype | is_valid
echo '^integer; X = T' | testtype | is_valid
echo '^integer; X = ^T' | testtype | is_valid
echo '^X; X = record A : integer end' | testtype | is_valid
echo '^X; X = integer' | testtype | is_valid

# Variable access
testvar() {
  echo 'program foo; type T = '; cat; echo "; var A : T; begin writeln($1) end."
}
echo 'integer' | testvar 'A' | is_valid
echo 'array[1..10] of integer' | testvar 'A[1]' | is_valid
echo 'array[1..10] of array[1..10] of integer' | testvar 'A[1][2]' | is_valid
echo 'array[1..10, 1..10] of integer' | testvar 'A[1][2]' | is_valid
echo 'array[1..10] of array[1..10] of integer' | testvar 'A[1, 2]' | is_valid
echo 'array[1..10, 1..10] of integer' | testvar 'A[1, 2]' | is_valid
echo 'record
        B : integer;
        C : record
              D : boolean;
              E : string
            end
      end' | testvar 'A.B' | is_valid
echo 'record
        B : integer;
        C : record
              D : boolean;
              E : string
            end
       end' | testvar 'A.C.D' | is_valid
echo 'record
        B : integer;
        C : record
              D : boolean;
              E : string
            end
      end' | testvar 'A.C.E' | is_valid
echo 'record
        B : integer;
        C : record
              D : boolean;
              E : string
            end
      end' | testvar 'A.C.F' | is_not_valid
echo 'array[1..10] of record
                        B : array[1..10] of record
                                              C : boolean
                                            end
                      end' | testvar 'A[1].B[2].C' | is_valid
echo '^integer' | testvar 'A^' | is_valid
echo 'integer' | testvar 'A^' | is_not_valid
echo 'program foo;
      type C = integer;
           T = ^C;
      var A : T;
      begin writeln(A^) end.' | is_valid
echo 'program foo;
      type T = ^C;
           C = integer;
      var A : T;
      begin writeln(A^) end.' | is_valid
echo 'program foo; 
      type T = record B : integer end;
           C = ^T;
      var A : C;
      begin writeln(A^.B) end.' | is_valid
echo 'program foo; 
      type T = record B : integer end;
           C = ^T;
      var A : C;
      begin writeln(A.B) end.' | is_not_valid
echo 'program foo; 
      type T = record C : ^integer end;
      type U = record B : ^T end;
           C = ^U;
      var A : C;
      begin writeln(A^.B^.C^) end.' | is_valid
echo 'program foo;
      type T = record C : array [1..10] of ^integer end;
      type U = array [1..10] of ^T;
      var A : ^U;
      begin writeln(A^[1]^.C[10]^) end.' | is_valid

# Procedures
echo 'program foo;
      procedure Proc;
      begin writeln(maxint) end;
      begin end.' | is_valid
echo 'program foo;
      procedure Proc();
      begin writeln(maxint) end;
      begin end.' | is_not_valid
echo 'program foo;
      procedure Proc(A : integer);
      begin writeln(maxint) end;
      begin end.' | is_valid
echo 'program foo;
      procedure Proc(A : integer; B : boolean);
      begin writeln(maxint) end;
      begin end.' | is_valid
echo 'program foo;
      procedure Proc(A, B : integer);
      begin writeln(maxint) end;
      begin end.' | is_valid
echo 'program foo;
      procedure Proc(var A : integer);
      begin writeln(maxint) end;
      begin end.' | is_valid
echo 'program foo;
      procedure Proc(var A : integer; B : boolean);
      begin writeln(maxint) end;
      begin end.' | is_valid
echo 'program foo;
      procedure Proc(var A, B : integer);
      begin writeln(maxint) end;
      begin end.' | is_valid
echo 'program foo;
      procedure Proc(const A : integer);
      begin writeln(maxint) end;
      begin end.' | is_valid
echo 'program foo;
      procedure Proc(const A : integer; B : boolean);
      begin writeln(maxint) end;
      begin end.' | is_valid
echo 'program foo;
      procedure Proc(const A, B : integer);
      begin writeln(maxint) end;
      begin end.' | is_valid
echo 'program foo;
      type T = procedure;
      procedure Proc(P : T);
      begin P end;
      begin end.' | will_be_valid
echo 'program foo;
      type T = procedure (A : integer);
      procedure Proc(P : T);
      begin P(1) end;
      begin end.' | will_be_valid
echo 'program foo;
      type T = function : integer;
      procedure Proc(P : T);
      begin writeln(P) end;
      begin end.' | will_be_valid
echo 'program foo;
      type T = function (A : integer) : integer;
      procedure Proc(P : T);
      begin writeln(P(1)) end;
      begin end.' | will_be_valid
echo 'program foo;
      procedure Proc; forward;
      procedure Proc;
      begin writeln(maxint) end;
      begin end.' | is_valid
echo 'program foo;
      procedure Proc(A : integer); forward;
      procedure Proc;
      begin writeln(maxint) end;
      begin end.' | is_valid
echo 'program foo;
      procedure Proc(A : integer); forward;
      procedure Proc(A : integer); forward;
      procedure Proc;
      begin writeln(maxint) end;
      begin end.' | is_not_valid
echo 'program foo;
      procedure Proc(A : integer); forward;
      procedure Proc(A : boolean);
      begin writeln(maxint) end;
      begin end.' | is_not_valid

# Functions
echo 'program foo;
      function Func : integer;
      begin Func := maxint end;
      begin end.' | is_valid
echo 'program foo;
      function Func() : integer;
      begin Func := maxint end;
      begin end.' | is_not_valid
echo 'program foo;
      function Func : array[1..10] of integer;
      begin Func[1] := 1 end;
      begin end.' | is_not_valid
echo 'program foo;
      type T = array[1..10] of integer;
      function Func : T;
      begin Func[1] := 1 end;
      begin end.' | will_be_valid
echo 'program foo;
      type T = ^integer;
      function Func : T;
      var Ret : ^integer;
      begin Func := Ret end;
      begin end.' | is_valid
echo 'program foo;
      function Func(A : integer) : integer;
      begin Func := maxint end;
      begin end.' | is_valid
echo 'program foo;
      function Func(A : integer; B : boolean) : integer;
      begin Func := maxint end;
      begin end.' | is_valid
echo 'program foo;
      function Func(A, B : integer) : integer;
      begin Func := maxint end;
      begin end.' | is_valid
echo 'program foo;
      function Func(var A : integer) : integer;
      begin Func := maxint end;
      begin end.' | is_valid
echo 'program foo;
      function Func(var A : integer; B : boolean) : integer;
      begin Func := maxint end;
      begin end.' | is_valid
echo 'program foo;
      function Func(var A, B : integer) : integer;
      begin Func := maxint end;
      begin end.' | is_valid
echo 'program foo;
      function Func : integer; forward;
      function Func : integer;
      begin Func := maxint end;
      begin end.' | is_valid
echo 'program foo;
      function Func(A : integer) : integer; forward;
      function Func;
      begin Func := maxint end;
      begin end.' | is_valid
echo 'program foo;
      function Func(A : integer) : integer; forward;
      function Func(A : integer) : integer; forward;
      function Func;
      begin Func := maxint end;
      begin end.' | is_not_valid
echo 'program foo;
      function Func(A : integer) : integer; forward;
      function Func : integer;
      begin Func := maxint end;
      begin end.' | is_not_valid
echo 'program foo;
      function Func(A : integer) : integer; forward;
      function Func(A : boolean) : integer;
      begin Func := maxint end;
      begin end.' | is_not_valid

# Expressions
testexpr() {
  echo "program foo; var R : $1; x : $2; y : $2; z : $2; begin R := $3 end."
}
testexpr integer integer x | is_valid
testexpr integer 'record A : integer; B : integer end' 'x.A' | is_valid
testexpr integer 'array [1..10] of integer' 'x[1]' | is_valid
testexpr integer integer 15 | is_valid
testexpr integer string 'length(x)' | is_valid
testexpr 'set of char' integer "['a'..'z']" | is_valid
testexpr integer integer '(x + y + z)' | is_valid
testexpr boolean boolean 'not x' | is_valid
testexpr integer integer 'x * y * z' | is_valid
testexpr real real 'x / 2' | is_valid
testexpr integer integer 'x div 2' | is_valid
testexpr integer integer 'x mod 2' | is_valid
testexpr boolean boolean 'x and y and z' | is_valid
testexpr integer integer 'x shl y' | is_valid
testexpr integer integer 'x shr y' | is_valid
testexpr integer integer '-x' | is_valid
testexpr integer integer 'x + y + z' | is_valid
testexpr integer integer 'x - y - z' | is_valid
testexpr boolean boolean 'x or y or z' | is_valid
testexpr boolean boolean 'x xor y xor z' | is_valid
testexpr boolean integer 'x = y' | is_valid
testexpr boolean integer 'x = y = z' | is_not_valid
testexpr boolean integer 'x > y' | is_valid
testexpr boolean integer 'x < y' | is_valid
testexpr boolean integer 'x >= y' | is_valid
testexpr boolean integer 'x <= y' | is_valid
testexpr boolean integer 'x <> y' | is_valid
testexpr boolean 'set of char' "'a' in x" | is_valid

# Statements
echo 'program foo; begin ; ; end.' | is_valid
echo 'program foo; var a : integer; begin a := 1 end.' | is_valid
echo 'program foo;
      function x : integer;
      begin x := 1 end;
      begin end.' | is_valid
echo 'program foo;
      procedure bar;
      begin end;
      begin bar end.' | is_valid
echo 'program foo;
      procedure bar(a : integer);
      begin end;
      begin bar(1) end.' | is_valid
echo 'program foo; var a : integer; b : integer;
      begin
      begin a := 1; b := 2 end;
      a := 3
      end.' | is_valid
echo 'program foo; var a : integer; b : integer;
      begin
      begin a := 1; b := 2; end;
      a := 3;
      end.' | is_valid
echo 'program foo; var a : integer; b : integer;
      begin if a = 1 then b := 2 end.' | is_valid
echo 'program foo; var a : integer; b : integer;
      begin if a = 1 then b := 2 else b := 3 end.' | is_valid
echo 'program foo; var a : integer; b : integer;
      begin if a = 1 then b := 2; else b := 3 end.' | is_not_valid
echo 'program foo; var a : integer; b : integer;
      begin if a = 1 then else b := 3 end.' | is_valid
echo "program foo; var a : char; b : integer;
      begin case a of 'a' : b := 1; 'b' : b := 2 end end." | is_valid
echo "program foo; var a : integer; b : integer;
      begin case a of 10 : b := 1; 20 : b := 2 end end." | is_valid
echo "program foo; var a : char; b : integer;
      begin case a of 'a' : b := 1; 'b' : b := 2; end end." | is_valid
echo "program foo; var a : char; b : integer;
      begin case a of 'a' : b := 1; else b := 2 end end." | is_valid
echo "program foo; var a : char; b : integer;
      begin case a of 'a' : b := 1; else b := 2; b := 3 end end." | is_valid
echo 'program foo; var a : integer; b : integer;
      begin
        a := 0; b := 10;
        repeat
          a := a + 1;
          b := b - 1
        until a > b
      end.' | is_valid
echo 'program foo; var a : integer; b : integer;
      begin
        a := 0; b := 10;
        while a <= b do
        begin
          a := a + 1;
          b := b - 1
        end
      end.' | is_valid
echo 'program foo; var a : integer; b : integer;
      begin
        b := 10;
        for a := 0 to 10 do b := b - 1
      end.' | is_valid
echo 'program foo; var a : integer; b : integer;
      begin
        b := 0;
        for a := 10 downto 0 do b := b + 1
      end.' | is_valid
echo 'program foo; var a : record b : integer end;
      begin
        with a do
        begin
          writeln(b)
        end
      end.' | is_valid
echo 'program foo; var a : record b : integer end; c : record d : integer end;
      begin
        with a, c do
        begin
          writeln(b);
          writeln(d)
        end
      end.' | is_valid

# I/O
testwrite() {
  echo "program foo; var $1; begin write($2) end."
}
testwrite 'a : boolean' a | is_valid
testwrite 'a : integer' a | is_valid
testwrite 'a : real' a | is_valid
testwrite 'a : char' a | is_valid
testwrite 'a : string' a | is_valid
testwrite 'a : string; b : string' 'a, b' | is_valid
testwrite 'a : string' 'output, a' | is_valid
testwrite 'a : string; b : string' 'output, a, b' | is_valid
testwrite 'a : integer' 'a:10' | is_valid
testwrite 'a : real' 'a:10:5' | is_valid
testwriteln() {
  echo "program foo; var $1; begin writeln($2) end."
}
testwriteln 'a : boolean' '' | is_valid
testwriteln 'a : boolean' 'output' | is_valid
testwriteln 'a : boolean' a | is_valid
testwriteln 'a : integer' a | is_valid
testwriteln 'a : real' a | is_valid
testwriteln 'a : char' a | is_valid
testwriteln 'a : string' a | is_valid
testwriteln 'a : string; b : string' 'a, b' | is_valid
testwriteln 'a : string' 'output, a' | is_valid
testwriteln 'a : string; b : string' 'output, a, b' | is_valid
testwriteln 'a : integer' 'a:10' | is_valid
testwriteln 'a : real' 'a:10:5' | is_valid
testread() {
  echo "program foo; var $1; begin read($2) end."
}
testread 'a : integer' a | is_valid
testread 'a : real' a | is_valid
testread 'a : char' a | is_valid
testread 'a : string' a | is_valid
testread 'a : string; b : string' 'a, b' | is_valid
testread 'a : string' 'input, a' | is_valid
testread 'a : string; b : string' 'input, a, b' | is_valid
testreadln() {
  echo "program foo; var $1; begin read($2) end."
}
testreadln 'a : integer' '' | is_valid
testreadln 'a : integer' 'input' | is_valid
testreadln 'a : integer' a | is_valid
testreadln 'a : real' a | is_valid
testreadln 'a : char' a | is_valid
testreadln 'a : string' a | is_valid
testreadln 'a : string; b : string' 'a, b' | is_valid
testreadln 'a : string' 'input, a' | is_valid
testreadln 'a : string; b : string' 'input, a, b' | is_valid
