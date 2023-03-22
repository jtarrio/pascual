#!/bin/bash

. ./testing.libsh

# Basic function calls.
echo "program foo;
      function Fun : string;
      begin Fun := 'fun' end;
      begin write(Fun) end." | outputs 'fun'
echo "program foo;
      function Fun : string;
      begin Fun := 'fun' end;
      begin write(Fun()) end." | outputs 'fun'
echo "program foo;
      function Fun(a : string; b : string) : string;
      begin Fun := 'fun ' + a + ' ' + b end;
      begin write(Fun('1', '2')) end." | outputs 'fun 1 2'
echo "program foo;
      function Fun(a : integer; b, c : string; d : integer) : string;
      begin Fun := 'fun ' + b + ' ' + c end;
      begin write(Fun(1, '2', '3', 4)) end." | outputs 'fun 2 3'

# The ways you can return a value
echo "program foo;
      function Foo : integer;
      begin Foo := 1234 end;
      begin write(Foo) end." | outputs '1234'
echo "program foo;
      function Foo : integer;
      begin Result := 1234 end;
      begin write(Foo) end." | outputs '1234'

# Arguments are passed by value
echo "program foo;
      var i : integer;
      function Fun(a : integer) : integer;
      begin
        a := 10;
        Fun := a + 5
      end;
      begin
        i := 1;
        write(Fun(i));
        write(' ', i)
      end." | outputs '15 1'
# Arguments can be passed by reference
echo "program foo;
      var i : integer;
      function Fun(var a : integer) : integer;
      begin
        a := 10;
        Fun := a + 5
      end;
      begin
        i := 1;
        write(Fun(i));
        write(' ', i)
      end." | outputs '15 10'
echo "program foo;
      var i, j : integer;
      function Swap(var a, b : integer) : boolean;
      var i : integer;
      begin
        i := a; a := b; b := i;
        Swap := a = b
      end;
      begin
        i := 1; j := 2;
        write(Swap(i, j));
        write(' ', i, ' ', j)
      end." | outputs 'FALSE 2 1'
# Arguments can be passed by const reference
echo "program foo;
      var i : integer;
      function Fun(const a : integer) : integer;
      begin
        Fun := a + 5
      end;
      begin
        i := 1;
        write(Fun(i))
      end." | outputs '6'
echo "program foo;
      var i : integer;
      function Fun(const a : integer) : integer;
      begin
        a := 10;
        Fun := a + 5
      end;
      begin
        i := 1;
        write(Fun(i))
      end." | is_not_valid
echo "program foo;
      function Fun(const a : integer) : integer;
      begin
        Fun := a + 5
      end;
      begin
        write(Fun(1))
      end." | outputs '6'
echo "program foo;
      function Fun(const a, b : string) : string;
      begin
        Fun := a + ' ' + b
      end;
      begin
        write(Fun('foo', 'bar'))
      end." | outputs 'foo bar'
# Variables and types can be defined inside the function
echo "program foo;
      function Fun(a : integer) : integer;
      type T = integer;
      var b : T;
      begin
        b := a + 1;
        Fun := 2 * b
      end;
      begin write(Fun(1)) end." | outputs '4'
# Variables and types are scoped and locals override globals
echo "program foo;
      type T = boolean;
      var b : char;
      var c : T;
      function Fun(a : integer) : integer;
      type T = integer;
      var b : T;
      begin
        b := a + 1;
        Fun := 2 * b
      end;
      begin
        b := 'b';
        c := true;
        write(b, ' ', c, ' ');
        write(Fun(1));
        write(' ', b, ' ', c)
      end." | outputs 'b TRUE 4 b TRUE'
# Functions can be called recursively
echo "program foo;
      function Fib(n : integer) : integer;
      var a : integer;
      begin
        if n <= 1 then Fib := 1
        else Fib := Fib(n - 1) + Fib(n - 2)
      end;
      begin write(Fib(5)) end." | outputs '8'
# Functions can be forward declared
echo "program foo;
      function Fun1(n : integer) : integer; forward;
      function Fun2(n : integer) : integer;
      begin
        if n > 1 then Fun2 := 1 + Fun1(n - 1)
        else Fun2 := 1
      end;
      function Fun1(n : integer) : integer;
      begin
        if n > 1 then Fun1 := 2 * Fun2(n - 1)
        else Fun1 := 1
      end;
      begin write(Fun1(4)) end." |
outputs '6'
echo "program foo;
      function Fun1(n : integer) : integer; forward;
      function Fun2(n : integer) : integer;
      begin
        if n > 1 then Fun2 := 1 + Fun1(n - 1)
        else Fun2 := 1
      end;
      function Fun1;
      begin
        if n > 1 then Fun1 := 2 * Fun2(n - 1)
        else Fun1 := 1
      end;
      begin write(Fun1(4)) end." |
outputs '6'
# Signature of implementation must match forward declaration
echo "program foo;
      function Fun(n : integer) : integer; forward;
      function Fun(n : string) : integer;
      begin end;
      begin end." | is_not_valid
echo "program foo;
      function Fun(n : integer) : integer; forward;
      function Fun(n : integer) : string;
      begin end;
      begin end." | is_not_valid

# Items passed by reference must be assignable
echo "program foo;
      function Make : integer;
      begin Make := 1 end;
      function Change(var t : integer) : integer;
      begin t := 2; Change := t end;
      begin write(Change(Make)) end." | is_not_valid
echo "program foo;
      const x : integer = 1;
      function Change(var t : integer) : integer;
      begin t := 2; Change := t end;
      begin write(Change(x)) end." | is_not_valid
# The result of a function is not assignable
echo "program foo;
      function Make : integer;
      begin Make := 1 end;
      begin Make := 2; write(Make) end." | is_not_valid
echo "program foo;
      function Make : integer;
      begin Make := 1 end;
      begin Make() := 2; write(Make()) end." | is_not_valid

# Passing functions in arguments
echo "program foo;
      type T = function(A : integer) : integer;
      function Duplicate(A : integer) : integer;
      begin Result := A + A end;
      procedure CallWithIntAndDisplay(P : T; A : integer);
      begin write(P(A)) end;
      begin CallWithIntAndDisplay(@Duplicate, 123) end." | outputs 246

# function-typed variables
echo "program foo;
      type T = function(A : integer) : integer;
      var Fun : T;
      function Duplicate(A : integer) : integer;
      begin Result := A + A end;
      begin Fun := @Duplicate; write(Fun(123)) end." | outputs 246

# 0-argument function variables are evaluated by write/writeln
echo "program foo;
      type T = function : integer;
      var Fun : T;
      function Gen42 : integer;
      begin Result := 42 end;
      begin Fun := @Gen42; write(Fun) end." | outputs 42
